#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("la grosse poutre");
    PlotFrame* F= T->add_frame();
    scalar l = 2;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);
    //auto M = std::make_shared<Mesh2>(mesh_generation::LloydRelaxation(convexPrimitive::Rectangle(3,1.5).subdivide(5),100));
    auto P = convexPrimitive::Rectangle(3,1).subdivide(5);
    auto M = std::make_shared<Mesh2>(mesh_generation::FromBoundaryMesh(P.getPointCloud(),0.05,P.getShapePredicate()));
    /*
    auto S = algo::FEM::LinearElasticity(M,{0,1,2,3,4,5},[](const vec&) {
        return linear_utils::vec2(0,-0.01);
    });
    */
    auto S = algo::FEM::LinearElasticity(M,{0,1,2,3,4,5,10,11,12,13,14,15},[](const vec&) {
        return linear_utils::vec2(0,-0.1);
    });
    /*
    w.add_mutable_scalar_by_cursor({0,3},"mu",100,[&S](scalar mu){
        S.setMu(mu);
        S.ComputeSolution();
    });
    w.add_mutable_scalar_by_cursor({0,3},"lambda",100,[&S](scalar l){
        S.setLambda(l);
        S.ComputeSolution();
    });
    */
    S.ComputeSolution();
    //L->addPlot<Mesh2DDisplayer>(M);
    L->addPlot<VectorValued2DMeshDisplayer>(M,S.getSolution())->set_dynamic();
    L->addPlot<Valued2DMeshDisplayer>(M,S.getSolutionNorm());
    //L->new_point_cloud(M->getVertexCloud(),2,true);
    w.show();
    return App.exec();
}
