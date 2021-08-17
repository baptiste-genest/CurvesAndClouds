#include "parametrization.h"

cnc::algo::geometry::LSCMParam::LSCMParam(cnc::algo::geometry::SimpleGLMesh *m, uint fixed_vertex_0, const QVector2D &p1, uint fixed_vertex_1, const QVector2D &p2)
{
    mesh = m;
    nb_vert = m->get_nb_vertices();
    nb_faces = m->get_nb_faces();
    fixed1 = fixed_vertex_0;
    fixed2 = fixed_vertex_1;
    u_coord.resize(nb_vert);
    v_coord.resize(nb_vert);
    u_coord[fixed1] = p1.x();
    u_coord[fixed2] = p2.x();
    v_coord[fixed1] = p1.y();
    v_coord[fixed2] = p2.y();
    M.resize(nb_faces,mat(2,3));
    compute_gradient_matrix_per_triangle();
}

cnc::scalar cnc::algo::geometry::LSCMParam::current_ELSCM_value() const
{
    scalar E = 0;
    vec v(3),u(3);
    for (uint f = 0;f<mesh->get_nb_faces();f++){
        get_v_value(f,v); get_u_value(f,u);
        E += mesh->face_area(f)*(M[f]*v - ortho*M[f]*u).norm2();
    }
    return E;
}

void cnc::algo::geometry::LSCMParam::get_v_value(uint f, cnc::vec &v) const
{
    auto F = mesh->get_face(f);
    v(0) = v_coord[F[0]];
    v(1) = v_coord[F[1]];
    v(2) = v_coord[F[2]];

}

void cnc::algo::geometry::LSCMParam::get_u_value(uint f, cnc::vec &u) const
{
    auto F = mesh->get_face(f);
    u(0) = u_coord[F[0]];
    u(1) = u_coord[F[1]];
    u(2) = u_coord[F[2]];

}

std::pair<QVector3D, QVector3D> cnc::algo::geometry::LSCMParam::get_triangle_orthobasis(uint f)
{
    auto F = mesh->get_face(f);
    QVector3D X = (mesh->get_vertex(F[1])->vpos - mesh->get_vertex(F[0])->vpos).normalized();
    QVector3D n = QVector3D::crossProduct(X,mesh->get_vertex(F[2])->vpos - mesh->get_vertex(F[0])->vpos).normalized();
    return {X,QVector3D::crossProduct(X,n)};
}

void cnc::algo::geometry::LSCMParam::compute_gradient_matrix_per_triangle()
{
    auto Me = mesh;
    for (uint f = 0;f<nb_faces;f++){
        auto B = get_triangle_orthobasis(f); qvr X = B.first,Y = B.second;
        auto dot = [Me] (uint f,uint i,const QVector3D& X) {
            return QVector3D::dotProduct(Me->get_vpos_by_face(f,i),X);
        };
        M[f](0,0) = dot(f,1,Y)-dot(f,2,Y);
        M[f](1,0) = dot(f,2,Y)-dot(f,0,Y);
        M[f](2,0) = dot(f,0,Y)-dot(f,1,Y);
        M[f](0,1) = dot(f,2,X)-dot(f,1,X);
        M[f](1,1) = dot(f,0,X)-dot(f,2,X);
        M[f](2,1) = dot(f,1,X)-dot(f,0,X);
        M[f] *= 0.5f/Me->face_area(f);
    }
}

cnc::scalar cnc::algo::geometry::LSCMParam::get_gradient_component(uint i)
{
    if (i == fixed1)
        return 0.f;
    if (i == fixed2)
        return 0.f;

}
