#include <curvesandcloud.h>

using namespace cnc;


int main(int argc, char *argv[])
{

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    L->add_mesh_displayer();

    w.show();
    return App.exec();
}
