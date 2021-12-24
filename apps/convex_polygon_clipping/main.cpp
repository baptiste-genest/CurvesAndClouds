#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotFrame* F = w.add_tab("convex polygon clipping")->add_frame();
    scalar r= 2;
    range R{-r,r};
    PlotLayer* L = F->add_grid_layer(R,R,false);

    ConvexPolygon P1,P2;
    P1 = convexPrimitive::Circle(1,20);
    //P2 = convexPrimitive::Square(1,vec({0.7,0.2}));
    P2 = convexPrimitive::Segment(vec({0.,0.}),vec({1.7,0.2}));

    auto CPP1 = P1.getIndexedCyclicPoints();
    auto CPP2 = P2.getIndexedCyclicPoints();
    /*
    L->new_point_cloud(cloud(CPP));
    */
    auto I = ConvexPolygonIntersection(P2,P1);
    auto CPPI = I.getIndexedCyclicPoints();
    L->new_2D_curve(CPP1,true);
    L->new_2D_curve(CPP2,true);
    L->new_2D_curve(CPPI,false);

    /*
    auto E = L->add_euclidean_plane(R,R);
    auto X = stat::random_var::PoissonSamplingInSphere(1,0.05,2,200);
    std::cout << X.size() << std::endl;
    for (const auto& x : X.points){
        auto pp = E->add_object<euclid::Point>(x,3);
        pp->set_color(QColorConstants::Blue);
        if (P.isInside(x))
            pp->set_color(QColorConstants::Red);
    }

    auto V = Voronoi(X,r);
    L->addPlot<DiagramPlotter>(V);
    */

    w.show();
    return App.exec();
}
