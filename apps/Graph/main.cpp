#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::graph;

int main(int argc, char *argv[])
{
    uint N = 20;
    Graph G(N,true);

    for (uint i = 0;i<40;i++){
        uint id = rand()%N;
        uint id2;
        do {
            id2 = rand()%N;
        } while(id == id2);
        G.addEdge(id,id2);
    }
    //G.addEdge(0,3);

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    auto GP = L->addGraphPlotter(G);

    w.show();
    return App.exec();
}
