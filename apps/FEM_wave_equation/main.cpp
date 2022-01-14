#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::linear_utils;

scalar ker(scalar x){
    if (std::abs(x) > 0.95)
        return 0;
    return std::exp(-1/(1-x*x));
}

scalar deduceLength(scalar A){
    return std::sqrt(4*A/sqrt(3));
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 6;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);
    MeshRef M;

    //auto P = convexPrimitive::Square(3).subdivide(5);
    std::string file = "../../data/rooms.mesh2";
    if (false){
        auto dC = geometry::CreateContext();
        dC->add_vertex(vec2(-1,1));
        dC->add_vertex(vec2(1,1));
        dC->add_vertex(vec2(1,2));
        dC->add_vertex(vec2(5,2));
        dC->add_vertex(vec2(5,-2));
        dC->add_vertex(vec2(1,-2));
        dC->add_vertex(vec2(1,-1));
        dC->add_vertex(vec2(-1,-1));
        dC->add_vertex(vec2(-1,-2));
        dC->add_vertex(vec2(-5,-2));
        dC->add_vertex(vec2(-5,2));
        dC->add_vertex(vec2(-1,2));
        scalar Ir = 0.08;
        scalar goalLength = 2*sqrt(3)*Ir;
        scalar goalArea = sqrt(3)*0.25*goalLength*goalLength;
        printf("Ir=%f,l=%f,A=%f\n",Ir,goalLength,goalArea);
        std::cout << Ir << ' ' << goalLength << ' ' << goalArea << std::endl;
        auto dM = Polygon(dC,dC->getIndexedVertices()).subdivideImposeLength(goalLength);
        auto B = dM.getPointCloud();
        auto p = dM.getShapePredicate();
        M = std::make_shared<Mesh2>(mesh_generation::FromBoundaryMesh(B,goalArea,p));
        algo::export_mesh2(*M,file);
    }
    else{
        M = algo::import_mesh2(file);
        M->computeConnectivity();
        //L->addPlot<Mesh2DDisplayer>(M);
        std::cout << "mesh done" << std::endl;
        std::cout << M->maxInscribedRadius() << std::endl;
    }

    vec U0(M.get()->interiorVertices().size()),V0(U0);
    uint i = 0;
    const auto& C = *M->getContext();
    for (auto v : M.get()->interiorVertices()){
        const auto& x = C(v);
        //U0(i) = x(0)<-1;
        //U0(i) = std::exp(-(x-vec2(-3)).norm()*10);
        U0(i) = ker((x-vec2(-3)).norm()*2);
        i++;
    }

    uint N = 300;
    auto S = FEM::WaveEquation(M,N,0.1);
    S.setInitialConditions(U0,V0);
    S.ComputeSolution();
    std::cout << "done computing" << std::endl;
    auto vr = S.SolutionValueRange();
    vr = {-std::abs(vr.second),vr.second};
    F->set_nb_layer_per_second(N);
    w.get_time_variable(5);
    S.setCursor(i);
    L->addPlot<Valued2DMeshDisplayer>(M,[&S](){
        S.ComputeSolution();
        return S.fullSolutionVector();
    },vr);

    w.show();
    return App.exec();
}
