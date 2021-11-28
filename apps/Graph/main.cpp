#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::graph;

int main(int argc, char *argv[])
{
    Graph G(5,true);

    for (uint i = 0;i<5;i++)
        G.addEdge(i,(i+1)%5);

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    L->addGraphPlotter(G);

    w.show();
    return App.exec();
}
