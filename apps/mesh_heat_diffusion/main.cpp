#include <curvesandcloud.h>
#include "head_diffusion.h"

using namespace cnc;
using namespace cnc::algo::geometry;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);
    PlotTab* T = w.add_tab("heat equation on mesh");

    auto F3D = T->add_3D_frame();
    F3D->set_ambient_light(false);
    F3D->set_color_mode(mesh_color_mode::given_color);

    F3D->load_mesh_from_obj("../../data/bunny.obj",0.6f);
    auto Mesh = F3D->get_mesh();

    heat_diffusion hd(Mesh,F3D->get_geometry_engine());

    w.show();
    return App.exec();
}


/*

    auto m = Mesh;
    auto LB = Mesh->compute_identity_plus_dt_sparse_laplace_beltrami_matrix(-.01);
    m->set_vertex_value(0,3000.f);
    auto v = Mesh->get_vertex_values();
    v = LB.conjugate_gradient(v,10.f);
    for (uint k = 0;k<Mesh->get_nb_vertices();k++)
        Mesh->set_vertex_value(k,v(k));

    QTimer* timer = new QTimer();
    auto lbr = &LB;
    w.connect(timer,&QTimer::timeout,[lbr,Mesh] (){
        auto v = Mesh->get_vertex_values();
        v = lbr->conjugate_gradient(v,1.f);
        for (uint k = 0;k<Mesh->get_nb_vertices();k++)
            Mesh->set_vertex_value(k,v(k));
    } );
    timer->start(100);
*/
