#include "head_diffusion.h"

heat_diffusion::heat_diffusion(SimpleGLMesh *m,GeometryEngine* ge) :  QObject(nullptr),v(m->get_nb_vertices()), M(m),GE(ge)
{
    v(0) = 3000.f;
    N = M->get_nb_vertices();
    LB = new smat(M->compute_identity_plus_dt_sparse_laplace_beltrami_matrix(dt));
    QTimer::singleShot(500,this,SLOT(compute()));
}

void heat_diffusion::compute() {
    v = LB->conjugate_gradient(v,1e-2);
    for (uint i = 0;i<N;i++)
        M->set_vertex_value(i,v(i));
    GE->declare_update_need();
    QTimer::singleShot(200,this,SLOT(compute()));
}
