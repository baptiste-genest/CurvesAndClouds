#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::geometry;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("3D plotting");

    MeshDisplayer* F3D = T->add_3D_frame();
    F3D->load_mesh_from_obj("../../data/nefertiti.obj",0.6);
    F3D->set_ambient_light(true);
    F3D->set_color_mode(mesh_color_mode::white);

    w.show();
    return App.exec();
}
