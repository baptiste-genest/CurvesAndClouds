#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;
//using namespace cnc;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar R = 2;
    range ER = {-(R+1),R+1};
    PlotLayer* L = F->add_grid_layer(ER,ER,false);


    auto Y = convexPrimitive::Circle(1,20).getIndexedPoints();
    //auto Y = algo::stat::random_var::sample_uniform_in_square(2,R,60);
    auto D = mesh_generation::BowyerWatson(Y);
    L->addPlot<Mesh2DDisplayer>(D);
    L->new_point_cloud(Y);


    w.show();
    return App.exec();
}
