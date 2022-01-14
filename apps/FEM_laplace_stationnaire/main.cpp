#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;



auto f = [] (const vec& x){
    if (x(0) > 0)
        return 1.;
    else
        return -1.;
};
/*
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
*/

//bool IterativeSolvers::debug = true;
int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    //PlotFrame* F= T->add_frame();
    scalar r = 3;
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

    using namespace linear_utils;
    auto G = CreateContext();
    auto& C = *G;
    C.add_vertex(vec2(-1,0));
    C.add_vertex(vec2(-1,-1));
    C.add_vertex(vec2(1,-1));
    C.add_vertex(vec2(1,1));
    C.add_vertex(vec2(0,1));
    C.add_vertex(vec2(0,0));
    auto P1 = Polygon(G,{0,1,2,3,4,5});
    auto B2 = P1.getPointCloud();
    auto p = P1.getShapePredicate();
    //auto M = mesh_generation::Refinement(B2,p,0.18);
    //auto M = std::make_shared<Mesh2>(mesh_generation::FromBoundaryMesh(B,0.05,P));

    std::string file = "../../data/rooms.mesh2";
    MeshRef M = algo::import_mesh2(file);
    M->computeConnectivity();
    //auto M = std::make_shared<Mesh2>(mesh_generation::LloydRelaxation(B[0],150));
    //auto M = std::make_shared<Mesh2>(mesh_generation::LaplacianRelaxation(B,P,300));
    //if (true){
        uint N = 3;
        auto S = algo::FEM::LaplaceEigenFunctions(M,N*N);
        S.ComputeSolution();
        for (uint i = 0;i<N*N;i++){
            auto L = T->add_frame_at(i%N,i/N)->add_grid_layer(R,R,false);
            S.setCursor(i);
            const auto& x = S.fullSolutionVector();
            L->addPlot<Valued2DMeshDisplayer>(M,x);
        }
        /*
    }
    else{
        uint N = 50;
        auto S = algo::FEM::LaplaceEigenFunctions(M,N);
        S.ComputeSolution();
        uint n =M->nbVertices();
        vec U0(n),Utilde(n);
        const auto& C = *M->getContext();
        M->computeConnectivity();
        for (auto v : M.get()->interiorVertices()){
            const auto& x = C(v);
            U0(v) = std::exp(-x.norm2());
        }
        std::vector<scalar> dots;
        for (uint i = 0;i<N;i++){
            S.setCursor(i);
            const auto& x = S.fullSolutionVector();
            Utilde += x*x.scalar_product(U0);
            dots.push_back(x.scalar_product(U0));
            std::cout << x.scalar_product(U0) << std::endl;
        }
        auto t = w.get_time_variable(20);
        T->add_frame()->add_grid_layer(R,R,false)->addPlot<Valued2DMeshDisplayer>(M,U0);
        auto up = [n,N,t,&S,dots] () {
            vec U(n);
            for (uint i = 0;i<N;i++){
                S.setCursor(i);
                U += S.fullSolutionVector()*std::cos(std::sqrt(S.getEigenValue())*t)*dots[i];
            }
            return U;
        };
        T->add_frame()->add_grid_layer(R,R,false)->addPlot<Valued2DMeshDisplayer>(M,up)->set_dynamic();
    }
    */



    w.show();
    return App.exec();
}
