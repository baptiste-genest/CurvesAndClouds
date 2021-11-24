#include <curvesandcloud.h>
#include "gridgraph.h"

using namespace cnc;

float height(float x,float y){
    return 1;
    return sin(x*M_PI)*sin(2*M_PI*y);
}

void gen_heightmap(int W,int H){
    std::ofstream f("heightmap.txt");

    f << W << ' ' << H << std::endl;

    float x,y;
    for (int j = 0;j<H;j++){
        y = float(j)/H;
        for (int i = 0;i<W;i++){
            x = float(i)/W;
            f << height(x,y) << ' ';
        }
        f << std::endl;
    }

    f.close();
}

int main(int argc, char *argv[])
{
    gen_heightmap(20,30);
    GridGraph G("heightmap.txt");
    G.perform_djikstra(0);
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    range r = {-0.1,1.1};
    auto EP = w.add_tab("graph")->add_frame()->add_layer()->add_euclidean_plane(r,r);

    G.plot(EP);

    w.show();
    return App.exec();
}
