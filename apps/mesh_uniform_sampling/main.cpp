#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;

void triangleSwap(cloud& X){
    for (auto& x : X.points){
        if (x(0) + x(1) > 1){
            x(0) = 1-x(0);
            x(1) = 1-x(1);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar r = 2;
    range R{-r-1,r+1};

    PlotLayer* L = F->add_grid_layer(R,R,false);
    //ConvexPolygon B1 = convexPrimitive::Circle(r,20);
    //ConvexPolygon B2 = convexPrimitive::Rectangle(2,1).subdivide(4);
    ConvexPolygon B1 = convexPrimitive::Rectangle(2,1).subdivide(4);
    auto M2 = mesh_generation::FromBoundaryMesh({B1},0.03,B1.getShapePredicate());
    M2.computeConnectivity();
    L->addPlot<Mesh2DDisplayer>(M2);

    w.show();
    return App.exec();
}


    /*
    auto X = algo::stat::random_var::PseudoRandomSequenceR2(50);
    triangleSwap(X);
    L->new_point_cloud(X);
    */
