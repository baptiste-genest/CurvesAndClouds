#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;
using namespace cnc::linear_utils;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    auto CR = CreateContext();
    auto& C = *CR;
    range r{-2,2};

    std::vector<vertex> PB;
    PB.push_back(C.add_vertex(vec2(-1,1)));
    PB.push_back(C.add_vertex(vec2(0,0)));
    PB.push_back(C.add_vertex(vec2(1,1)));
    PB.push_back(C.add_vertex(vec2(1,0)));
    PB.push_back(C.add_vertex(vec2(0,-1)));
    PB.push_back(C.add_vertex(vec2(-1,0)));

    Polygon P(CR,PB);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_grid_layer(r,r,false);
    L->new_2D_curve(P.getIndexedCyclicPoints(),true);
    cloud X = algo::stat::random_var::sample_uniform_in_square(2,2,100).filter(P.getShapePredicate());
    L->new_point_cloud(X);

    w.show();
    return App.exec();
}
