#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::geometry;

void color_by_gaussian_curvature(MeshDisplayer* F){
    F->set_color_mode(given_color);
    auto M = F->get_mesh();
    vec C = M->gaussian_curvature();
    auto R = algo::calculus::build_range_mapper({0,1},{0,255});
    auto B = algo::calculus::build_range_mapper({-1,0},{255,0});
    for (uint k = 0;k<M->get_nb_vertices();k++)
        M->set_vertex_color(k,QVector3D(R(C(k)),0,B(C(k))));
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("fucking 3D RABBIT");
    auto F1= T->add_3D_frame();
    F1->load_mesh_from_obj("/home/eulerson/Documents/test.obj");
    auto F2= T->add_3D_frame();
    F2->load_mesh(stanford_bunny);

    color_by_gaussian_curvature(F1);
    color_by_gaussian_curvature(F2);


    w.show();
    return App.exec();
}

