#include <curvesandcloud.h>
#include "gridgraph.h"

using namespace cnc;

float height(float x,float y){
    return (y>0.5f)*sin(x*5*M_PI)*sin(y*5*M_PI);
    return sin(x*M_PI)*sin(y*M_PI);//*sin(M_PI*y*2);
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
    gen_heightmap(30,30);
    GridGraph G("heightmap.txt");
    G.perform_djikstra(G.get_index(0,15));
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    auto T = w.add_tab("graph");
    range r = {-0.1,1.1};
    auto EP = T->add_frame()->add_layer()->add_euclidean_plane(r,r);
    auto HM = T->add_frame()->add_layer()->new_colormap(height,{0,1},{0,1},color_policy::from_zero);

    G.plot(EP,30*30-1);

    w.show();
    return App.exec();
}
