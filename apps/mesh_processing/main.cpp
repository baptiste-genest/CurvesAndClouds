#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::geometry;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("3D plotting");

    MeshDisplayer* F3D = T->add_3D_frame();
    F3D->load_mesh_from_obj("../../data/bunny.obj",0.6f);
    F3D->set_ambient_light(true);
    F3D->set_color_mode(mesh_color_mode::white);

    auto M = F3D->get_mesh();
    smat LB = M->compute_identity_plus_dt_sparse_laplace_beltrami_matrix(0.1);


    w.show();
    return App.exec();
}
