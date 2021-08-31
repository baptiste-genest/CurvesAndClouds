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

scalar smallest_eigenvalue(const mat& A){
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

void gradient_descent_flat_energy(SimpleGLMesh* m,scalar eps) {
    const scalar dx = 1e-4,idx = 1/dx;
    scalar dt = 1e-3;
    QVector3D Dx[3];
    for (uint i = 0;i<3;i++)
        Dx[i][i] = dx;
    std::vector<QVector3D> gradients(m->get_nb_vertices());
    std::vector<std::vector<uint>> N(m->get_nb_vertices());
    for (uint k = 0;k<m->get_nb_vertices();k++){
        N[k] = m->one_ring_neighbors(k);
        N[k].push_back(k);
    }

    scalar E = 30,old_E;
    std::vector<scalar> old_values(m->get_nb_vertices());
    while (E > eps){
        old_E = E;
        E = 0;
        for (uint k = 0;k<m->get_nb_vertices();k++)
             old_values[k] = get_flat_energy(m,k);
        for (uint k = 0;k<m->get_nb_vertices();k++){
            QVector3D old_pos = m->get_vpos(k);
            for (uint i = 0;i<3;i++){
                m->set_vertex_pos(k,old_pos + Dx[i]);
                gradients[k][i] = 0;
                for (uint n : N[k])
                    gradients[k][i] += (get_flat_energy(m,n)-old_values[n])*idx;
                m->set_vertex_pos(k,old_pos);
            }
            //std::cout << gradients[k].length() << std::endl;
            E += old_values[k];
        }
        for (uint k = 0;k<m->get_nb_vertices();k++){
            QVector3D old_pos = m->get_vpos(k);
            m->set_vertex_pos(k,old_pos - gradients[k]*dt);
        }
        std::cout << E << std::endl;
        if (old_E < E)
            dt *= 0.9;
    }
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("Developability");

    MeshDisplayer* F3D = T->add_3D_frame();
    F3D->load_mesh(face_mask);
    F3D->set_color_mode(mesh_color_mode::white);
    F3D->set_light_pos(QVector3D(1,-5,-10));
    F3D->set_ambient_light(true);
    auto mesh = F3D->get_mesh();
    gradient_descent_flat_energy(mesh,1);
    mesh->compute_normals();

    std::string out = "/home/eulerson/Documents/test.obj";
    F3D->get_mesh()->export_as_obj(out);

    w.show();
    return App.exec();
}
