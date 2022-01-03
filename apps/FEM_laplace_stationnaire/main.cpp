#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;



void cnc::algo::FEM::LaplaceEigenFunctions::ComputeSolution()
{
    const auto& C = *M.getContext();
    const auto& IV = M.getInteriorVertices();
    SMB MassMatrix(IV.size(),true);
    for (const auto& e : M.interiorEdges()){
        int ids[2];
        bool interior = true;
        for (uint i = 0;i<2;i++){
            auto it = std::find(IV.begin(),IV.end(),e[i]);
            if (it == IV.end())
                interior = false;
            else
                ids[i] = std::distance(IV.begin(),it);
        }
        if (interior){
            scalar S = 0;
            for (const auto& f : M.getAdjacentFaces(e)){
                const auto& G1 = B[e[0]][f];
                const auto& G2 = B[e[1]][f];
                scalar s = 0;
                if (true){
                    for (const auto& me : f){
                        auto m = C.midPoint(me);
                        s += G1(m)*G2(m)*0.33333;
                    }
                }
                else
                    s = G1(C.midPoint(f))*G2(C.midPoint(f));
                S += s*C.faceArea(f);
            }
            MassMatrix(ids[0],ids[1]) = S;
        }
    }
    uint id = 0;
    for (auto it = IV.begin();it != IV.end();++it){
        scalar SG = 0;
        for (const auto& psi : B[*it]){
            scalar s = 0;
            if (true){
                for (const auto& me : psi.first){
                    auto m = C.midPoint(me);
                    s += psi.second(m)*psi.second(m)*0.33333;
                }
            }
            else
                s = psi.second(C.midPoint(psi.first))*psi.second(C.midPoint(psi.first));
            SG += s*C.faceArea(psi.first);
        }
        MassMatrix(id,id) = SG;
        id++;
    }
    auto RM = RigidityMatrix.DenseMatrix();
    auto MM = MassMatrix.DenseMatrix();
    auto L = algo::Cholesky(MM);
    //auto IL = L.invert_triangular(LOWER);
    auto ILR = algo::Cholesky(RM).invert_triangular(LOWER);
    auto IRM = ILR.transpose()*ILR;
    //auto K = IL*RM*L.transpose().invert_triangular(UPPER);
    auto K = L.transpose()*IRM*L;
    //auto eigen = K.power_method(algo::ones(IV.size()),1e-30);
    auto eigen = K.lanczos(nth)[nth-1];
    //auto eigen = algo::IterativeSolvers::InversePowerMethod(K,1e-10);
    //eigen = algo::IterativeSolvers::InversePowerMethod(K*chaskal::ortho_proj_matrix<scalar>({eigen.vector}),1e-10,true);
    std::cout << (eigen.vector*eigen.value - K*eigen.vector).norm_inf() << std::endl;
    solution = L.transpose().upper_solve(eigen.vector);
    //std::cout << (vec(RM*solution) - MM*solution*eigen.value).norm_inf() << std::endl;
    std::cout << "associated eigen value: " << 1.0/eigen.value << std::endl;
    auto r = SolutionValueRange();
    printf("%f from %f\n",r.first,r.second);
}
auto f = [] (const vec& x){
    if (x(0) > 0)
        return 1.;
    else
        return -1.;
};
using FEMS = algo::FEM::FiniteElementSolver;

void plotIsoLines(PlotLayer* L,const FEMS& F){
    const static auto ortho = [] (const vec& e){
        return vec({e(1),-e(0)});
    };
    const auto& M = F.getMesh();
    const auto& C = *M.getContext();
    auto z_range = F.SolutionValueRange();
    auto R = algo::calculus::build_range_mapper({0,z_range.second},{0,255},true);
    auto B = algo::calculus::build_range_mapper({z_range.first,0},{255,0},true);
    for (const auto& f : M.faces()){
        auto cf = f;
        auto ce = cf[0];
        std::vector<vec> iso_line;
        vec cx = C(ce[0])+C.get_vec_edge(ce,ce[0])*stat::random_var::random_scalar(0,1);
        bool end = false;
        scalar z = F.sampleSolutionOnFace(cx,cf);
        for (uint i = 0;i<10 && !end;i++){
            iso_line.push_back(cx);
            auto others = get_other_edges(cf,ce);
            for (const auto& e : others){
                bool I;
                auto grad = F.SolutionGradientOnFace(cf);
                if (grad.norm() < 1e-8){
                    end = true;
                    break;
                }
                auto nx = raySegmentIntersection(cx,ortho(grad),C(e[0]),C(e[1]),I);
                if (I){
                    cx = nx;
                    ce = e;
                    try {
                        cf = M.getOppositeFace(cf,ce);
                    }  catch (Cnc_error& e) {
                        end = true;
                        break;
                    }
                }
            }
        }
        L->new_2D_curve(iso_line)->set_color(QColor::fromRgb(R(z),0,B(z)));
    }
}

//bool IterativeSolvers::debug = true;
int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    //PlotFrame* F= T->add_frame();
    scalar r = 2;
    range R{-r-1,r+1};
    //PlotLayer* L = F->add_grid_layer(R,R,false);

    std::vector<ConvexPolygon> B;
    ShapePredicate P;
    if (false){
        B.push_back(convexPrimitive::Rectangle(2*r,r).subdivide(5));
        B.push_back(convexPrimitive::Circle(r/4,20));
        P = Inter(B[0].getShapePredicate(),Not(B[1].getShapePredicate()));
        //P = B[0].getShapePredicate();
    }
    else {
        B.push_back(convexPrimitive::Square(2*r).subdivide(8));
        //B.push_back(convexPrimitive::Circle(r,20));
        P = B[0].getShapePredicate();
    }
    //auto S = algo::FEM::LaplaceEigenFunctions(mesh_generation::LaplacianRelaxation(B,P,200),4);
    //auto M = mesh_generation::LaplacianRelaxation(B,P,130);
    //auto S = algo::FEM::LaplaceEigenFunctions(,8);

    auto M = std::make_shared<Mesh2>(mesh_generation::FromBoundaryMesh(B,0.05,P));
    for (uint i = 0;i<9;i++){
        auto L = T->add_frame_at(i%3,i/3)->add_grid_layer(R,R,false);
        auto S = algo::FEM::LaplaceEigenFunctions(M,i+1);
        S.ComputeSolution();
        L->addPlot<FEMDisplayer>(S);
        //plotIsoLines(L,S);
    }
    /*
    L->addPlot<Mesh2DDisplayer>(S.getMesh());
    T->add_frame()->add_layer()->new_colormap([&S](scalar x,scalar y){
    return S.sampleSolution(vec({x,y}));
    },R,R,color_policy::from_zero);
    */
    w.show();
    return App.exec();
}
