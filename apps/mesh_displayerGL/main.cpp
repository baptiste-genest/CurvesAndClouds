#include <curvesandcloud.h>

using namespace cnc;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("fucking 3D RABBIT");
    auto F= T->add_3D_frame();
    //F->load_mesh_from_obj("/home/eulerson/Documents/test.obj");
    F->load_mesh(face_mask);

    if (true) {
        auto M = F->get_mesh();
        vec C;
        if (false){
            F->set_color_mode(mesh_color_mode::interpolate_value);
            C = M->mean_curvature();
            M->set_vertex_values(C.scale_from_0_to_1());
        }
        else {
            F->set_color_mode(given_color);
            C = M->gaussian_curvature();
            auto R = algo::calculus::build_range_mapper({0,1},{0,255});
            auto B = algo::calculus::build_range_mapper({-1,0},{255,0});
            for (uint k = 0;k<M->get_nb_vertices();k++)
                M->set_vertex_color(k,QVector3D(R(C(k)),0,B(C(k))));
        }
    }
    else {
        F->set_light_pos(QVector3D(1,-5,-10));
        F->set_ambient_light(true);
        //F->set_specular_reflection(true);
    }

    w.show();
    return App.exec();
}
