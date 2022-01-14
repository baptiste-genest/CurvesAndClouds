#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::linear_utils;
using namespace cnc::algo::geometry;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);
    PlotTab* T = w.add_tab("Mesh Operators");

    if (false){
        range r{-1.5,1.5};
        PlotLayer* L1 = T->add_frame()->add_grid_layer(r,r,false);
        PlotLayer* L2 = T->add_frame()->add_grid_layer(r,r,false);
        auto GR = std::make_shared<GeometricContext>();
        auto& G = *GR;
        G.add_vertex(vec2(-1,0));
        G.add_vertex(vec2(0,0.5));
        G.add_vertex(vec2(0,-0.5));
        G.add_vertex(vec2(1,0));
        auto F1 = topology::assemble_face(0,1,2);
        auto F2 = topology::assemble_face(3,1,2);
        auto M1 = std::make_shared<Mesh2>(GR,topology::faces{F1,F2});
        L1->addPlot<Mesh2DDisplayer>(M1);
        auto M2 = std::make_shared<Mesh2>(*M1);
        M2->computeConnectivity();
        M2->edgeSplit({1,2});
        M2->edgeSplit({0,1});
        M2->edgeCollapseInto({1,2},1);
        M2->filterDeadVertices();
        L2->addPlot<Mesh2DDisplayer>(M2);
        L2->new_point_cloud(M2->getVertexCloud(),2,true);
    }
    else {
        scalar l = 2;
        range r{-l,l};

        auto G = CreateContext();
        cloud B;
        ShapePredicate p;
        auto P = convexPrimitive::Square(1);
        //auto P = convexPrimitive::Circle(1.,20);
        B = P.getPointCloud();
        p = P.getShapePredicate();
        auto M = mesh_generation::Refinement(B,p,0.15);
        cloud MF;
        for (auto v : M->mf)
            MF.add_point(M->getVertexPosition(v));
        auto L = T->add_frame()->add_grid_layer(r,r,false);
        L->new_point_cloud(M->getVertexCloud(),2,true);
        L->addPlot<Mesh2DDisplayer>(M);
        L->new_point_cloud(MF,2,true);
    }

    w.show();
    return App.exec();
}
/*

    {
    }
    {
    range r{-2.5,2.5};
    PlotLayer* L1 = T->add_frame_at(0,1)->add_grid_layer(r,r,false);
    PlotLayer* L2 = T->add_frame_at(1,1)->add_grid_layer(r,r,false);
    auto GR = std::make_shared<GeometricContext>();
    auto& G = *GR;
    for (uint i = 0;i<6;i++){
        scalar th = M_PI*2*i/6;
        G.add_vertex(vec2(cos(th),sin(th)));
    }
    for (int i = 0;i<3;i++){
        scalar th = M_PI*2*(i-1)/6;
        G.add_vertex(vec2(cos(th)+1,sin(th)));
    }
    auto c1 = G.add_vertex(vec2());
    topology::faces F;
    for (uint i = 0;i<6;i++){
        F.insert(topology::assemble_face(i,(i+1)%6,c1));
    }
    for (uint i = 0;i<3;i++)
        F.insert(topology::assemble_face(0,5+i,6+i));
    F.insert(topology::assemble_face(0,1,8));
    auto M1 = std::make_shared<Mesh2>(GR,F);
    M1->computeConnectivity();
    L1->addPlot<Mesh2DDisplayer>(M1);
    auto M2 = std::make_shared<Mesh2>(*M1);
    M2->computeConnectivity();
    M2->edgeCollapseInto({1,8},1);
    //M2->edgeCollapse({c1,0});
    L2->addPlot<Mesh2DDisplayer>(M2);
    }


    */
