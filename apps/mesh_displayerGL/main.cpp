#include <curvesandcloud.h>

using namespace cnc;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("fucking 3D RABBIT");
    auto F= T->add_3D_frame();
    if (true)
    {
        F->load_mesh_from_obj("../../data/bunny.obj",0.6f);
        F->set_color_mode(mesh_color_mode::given_color);
        /*
        F->set_ambient_light(true);
        F->set_specular_reflection(true);
        */
        auto mesh = F->get_mesh();
        mesh->truncate_mesh([] (const QVector3D& x) {
            return x.z() < 0.2;
        });
        mesh->compute_normals();
        auto B = mesh->get_boundary_vertices();
        for (auto id : B)
            mesh->set_vertex_color(id,QVector3D(1.,0.,0.));
    }
    else
    {
        F->load_mesh_from_obj("../../data/bunny.obj",0.6f);
        F->set_color_mode(mesh_color_mode::interpolate_value);
        auto M = F->get_mesh();
        auto C = M->test();
        std::cout << C.norm_inf() << std::endl;
        M->set_vertex_values(C.scale_from_0_to_1());
    }

    w.show();
    return App.exec();
}
