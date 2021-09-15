#include <curvesandcloud.h>

using namespace cnc;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("Tree simulation");
    auto SV = T->add_3D_scene();

    w.show();
    return App.exec();
}
