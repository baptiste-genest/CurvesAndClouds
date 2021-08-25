#include "head_diffusion.h"

heat_diffusion::heat_diffusion(SimpleGLMesh *m,GeometryEngine* ge) :  QObject(nullptr),v(m->get_nb_vertices()), M(m),GE(ge)
{
    v(0) = 3000.f;
    N = M->get_nb_vertices();
    LB = new smat(M->compute_weight_plus_dt_cot_matrix(dt));
    W = new smat(M->compute_mass_matrix());
    QTimer::singleShot(500,this,SLOT(compute()));
}

void heat_diffusion::compute() {
    v = LB->conjugate_gradient(W->mult(v),1e-2);
    for (uint k = 0;k<N;k++){
        scalar val = algo::clamp<scalar>(v(k),0,1);
        M->set_vertex_color(k,QVector3D(val,0,1-val));
    }
    M->set_vertex_values(v);
    GE->declare_update_need();
    QTimer::singleShot(200,this,SLOT(compute()));
}
