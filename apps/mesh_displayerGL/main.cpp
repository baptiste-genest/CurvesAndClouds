#include <curvesandcloud.h>

using namespace cnc;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("fucking 3D RABBIT");
    /*
    {
        auto F= T->add_3D_frame();
        F->load_mesh_from_obj("/home/eulerson/Documents/armadillo.obj",0.6f);
    }
    */
    {
        auto F= T->add_3D_frame();
        F->load_mesh_from_obj("../../data/bunny.obj",0.6f);
        F->set_ambient_light(true);
        F->set_specular_reflection(true);
        //auto M = F->get_mesh();
        //auto L = M->compute_laplace_beltrami_matrix();
        /*
        auto C = M->test();
        range minmax = algo::get_min_max_range(C);
        std::cout << minmax.first << ' ' << minmax.second << std::endl;
        auto map = algo::calculus::build_range_mapper(minmax,{0.f,1.f});
        for (uint k = 0;k<M->get_nb_vertices();k++)
            M->set_vertex_color(k,QVector3D(map(C[k]),0.f,1.f-map(C[k])));
            */
    }

    w.show();
    return App.exec();
}
