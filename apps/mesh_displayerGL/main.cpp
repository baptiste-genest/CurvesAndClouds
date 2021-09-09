#include <curvesandcloud.h>

using namespace cnc;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("fucking 3D RABBIT");
    auto F= T->add_3D_frame();
    //F->load_mesh_from_obj("/home/eulerson/Documents/test.obj");
    F->load_mesh(bank_mesh_names::stanford_bunny);

    if (true) {
        auto M = F->get_mesh();
        vec C;
        if (true){
            F->set_color_mode(mesh_color_mode::interpolate_pos_value);
            C = M->mean_curvature();
            M->set_vertex_values(C.scale_from_0_to_1()*10);
        }
        else {
            F->set_color_mode(mesh_color_mode::interpolate_value);
            C = M->gaussian_curvature()*255.f;
            M->set_vertex_values(C);
        }
    }
    else {
        auto M = F->get_mesh();
        M->compute_normals();
        F->set_light_pos(QVector3D(1,-5,-10));
        F->set_ambient_light(true);
        F->set_specular_reflection(true);
    }

    w.show();
    return App.exec();
}
