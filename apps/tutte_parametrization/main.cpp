#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::geometry;

vec qtv(const QVector3D& x){
    return vec({x.x(),x.y(),x.z()});
}

#define rabbit

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("uv mapping");

    MeshDisplayer* F3D = T->add_3D_frame();
    F3D->set_ambient_light(true);
    F3D->set_specular_reflection(true);
    F3D->set_color_mode(mesh_color_mode::white);

#ifdef nefertiti
    F3D->load_mesh_from_obj("../../data/nefertiti.obj",0.6);
#endif
#ifdef rabbit
    F3D->load_mesh_from_obj("../../data/bunny.obj",0.6);
#endif

    auto mesh = F3D->get_mesh();

#ifdef rabbit
    QVector3D aim = QVector3D(-0.25,1.,0).normalized();
    scalar pval = 1;
    mesh->truncate_mesh([aim,pval] (qvr x) {
        return QVector3D::dotProduct(aim,x) < pval;
    });
    vec n = qtv(aim);
#endif


    auto B = mesh->get_boundary_vertices();
    std::sort(B.begin(),B.end());
    auto L = mesh->compute_sparse_laplace_beltrami_matrix(false);

    cloud mp;
    for (uint i : B)
        mp.add_point(qtv(mesh->get_vpos(i)));

#ifdef rabbit
    mp = algo::stat::project_into_2D(center(algo::stat::project_on_plane(mp,n,pval)));
#endif
#ifdef nefertiti
    mp = algo::stat::project_into_2D(mp);
#endif

    mp.apply([] (const vec& x) {
        return x.normalize();
    });

    std::vector<scalar> u_known(B.size());
    std::vector<scalar> v_known(B.size());

    for (uint i = 0;i<B.size();i++){
        auto p = mp[i].normalize();
        u_known[i] = p(1);
        v_known[i] = -p(0);
    }

    auto P = algo::set_known_variables(L,B);
    vec u = algo::solve_for_kernel_with_known_variables(P,mesh->get_nb_vertices(),B,u_known,1e-5).scale_from_0_to_1();
    vec v = algo::solve_for_kernel_with_known_variables(P,mesh->get_nb_vertices(),B,v_known,1e-5).scale_from_0_to_1();

    range r({0,1});
    auto layer = T->add_frame()->add_grid_layer(r,r,false);
    for (uint f = 0;f<mesh->get_nb_faces();f++){
        auto fid = mesh->get_face(f);
        vec v1 = vec({u(fid[0]),v(fid[0])});
        vec v2 = vec({u(fid[1]),v(fid[1])});
        vec v3 = vec({u(fid[2]),v(fid[2])});
        auto C = layer->new_2D_curve({v1,v2,v3,v1});
        C->set_color(QColorConstants::Black);
        C->set_line_width(1);
    }

    w.show();
    return App.exec();
}
