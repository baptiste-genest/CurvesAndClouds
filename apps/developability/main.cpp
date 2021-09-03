#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::geometry;

mat tensorize(const QVector3D& v){
    mat T(3,3);
    for (uint j = 0;j<3;j++)
        for (uint i = 0;i<3;i++)
            T(i,j) = v[i]*v[j];
    return T;
}
mat tensorize(const QVector3D& x,const QVector3D& y){
    mat T(3,3);
    for (uint j = 0;j<3;j++)
        for (uint i = 0;i<3;i++)
            T(i,j) = x[j]*y[i];
    return T;
}


scalar smallest_eigenvalue(const mat& A){
    return algo::symetric_3x3_eigenvalue_solver(A)[2];
    try {
        mat I = A.invert();
        return 1/I.power_method().value;
    }  catch (MatrixException) {
        return 0;
    }
    return 0;
}

scalar get_flat_energy(SimpleGLMesh* m,uint i){
    auto Ve = m->get_vertex(i)->emerging;
    mat SNNt(3,3);
    for (auto h : Ve)
        SNNt += tensorize(m->get_face_normal(h->face_id))*m->angle_from_incident_angle(h);
    return smallest_eigenvalue(SNNt);
}

QVector3D angle_gradient(SimpleGLMesh* m,halfedge* h,const QVector3D& N){
    qvr fi = m->get_vpos(h->prev->vertex_id);
    qvr fj = m->get_vpos(h->vertex_id);
    qvr fk = m->get_vpos(h->next->vertex_id);
    return -QVector3D::crossProduct(N,(fi-fj).normalized())-QVector3D::crossProduct(N,(fk-fi).normalized());
}

mat normal_gradient(SimpleGLMesh* m,halfedge* h,const QVector3D& N) {
    scalar area = m->face_area(h->face_id);
    qvr fj = m->get_vpos(h->vertex_id);
    qvr fk = m->get_vpos(h->next->vertex_id);
    return tensorize(QVector3D::crossProduct(fk-fj,N),N)*(1/area);
}

QVector3D apply(const mat& A,const QVector3D& x){
    QVector3D b;
    for (uint j = 0;j<3;j++)
        for (uint i = 0;i<3;i++)
            b[j] += A(i,j)*x[i];
    return b;
}

QVector3D compute_gradient(SimpleGLMesh* m,uint p,scalar& E) {
    auto v = m->get_vertex(p);
    QVector3D grad;
        //std::cout << E << std::endl;
    mat T(3,3);
    uint N = v->emerging.size();
    std::vector<QVector3D> normals(N);
    std::vector<scalar> angles(N);
    uint id = 0;
    for (halfedge* h : v->emerging){
        QVector3D N = m->get_face_normal(h->face_id);
        scalar th = m->angle_from_incident_angle(h);
        T += tensorize(N)*th;
        normals[id] = N;
        angles[id] = th;
        id++;
    }
    scalar eigval = smallest_eigenvalue(T);
    QVector3D x = algo::symetric_3x3_eigenvector(T,eigval);
    E += eigval;
    id = 0;
    for (halfedge* h : v->emerging){
        qvr N = normals[id];
        scalar th = angles[id];
        scalar dot = QVector3D::dotProduct(x,N);
        grad += algo::sq(dot)*angle_gradient(m,h,N) +
                apply(normal_gradient(m,h,N).transpose(),x)*(2*dot*th);
        id++;
    }
    return grad;
}

std::vector<scalar> gradient_descent_flat_energy(SimpleGLMesh* m,scalar eps) {
    scalar dt = 5e-6;
    std::vector<QVector3D> gradients(m->get_nb_vertices());
    std::vector<std::vector<uint>> N(m->get_nb_vertices());
    std::vector<scalar> history;
    for (uint k = 0;k<m->get_nb_vertices();k++){
        N[k] = m->one_ring_neighbors(k);
        N[k].push_back(k);
    }

    scalar E = 2*eps,old_E = 10000;
    for (uint iter = 0; E > eps;iter++){
        if (E > old_E && iter > 10)
            dt *= 0.9;
            //return history;
        if (iter)
            history.push_back(E);
        old_E = E;
        E = 0;
        for (uint k = 0;k<m->get_nb_vertices();k++)
            gradients[k] = compute_gradient(m,k,E);
        for (uint k = 0;k<m->get_nb_vertices();k++){
            qvr old_pos = m->get_vpos(k);
            m->set_vertex_pos(k,old_pos - gradients[k]*dt);
        }
        std::cout << E << std::endl;
    }
    return history;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("Developability");

    MeshDisplayer* F3D = T->add_3D_frame();
    F3D->load_mesh(stanford_bunny);
    F3D->set_color_mode(mesh_color_mode::white);
    F3D->set_light_pos(QVector3D(1,-5,-10));
    F3D->set_ambient_light(true);
    auto mesh = F3D->get_mesh();
    auto hist = gradient_descent_flat_energy(mesh,66);
    mesh->compute_normals();

    {
        T->add_frame()->add_layer()->new_2D_curve(algo::calculus::get_lin_space(0,1,(int)hist.size()),hist);
    }

    std::string out = "/home/eulerson/Documents/test.obj";
    F3D->get_mesh()->export_as_obj(out);

    w.show();
    return App.exec();
}
