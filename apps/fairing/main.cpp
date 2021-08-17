#include <curvesandcloud.h>

using namespace cnc;

namespace constants {
int layers = 20,resolution = 30;
}

std::pair<std::vector<uint>,std::vector<QVector3D>> build_cylinder(algo::geometry::SimpleGLMesh* M){
    using namespace constants;
    using namespace std;
    QVector3D offset(0.f,0.f,0.f),p;
    float scale = 0.6f,r = 0.3f;
    std::vector<uint> ids;
    std::vector<QVector3D> pos;
    QQuaternion R;
    int thick = 3;
    for (int j = 0;j<layers;j++){
        float z = float(j)/(layers-1);
        int flid = j*resolution;
        R = QQuaternion::fromAxisAndAngle(1.f,0.f,0.f,z*90);
        offset.setZ(z);
        for (int i = 0;i<resolution;i++){
            int id = flid+i;
            float th = 2*M_PI*i/resolution;
            p = R*(QVector3D(cos(th)*r,sin(th)*r,z)*scale) + offset;
            M->set_vertex_pos(id,p);
            M->set_vertex_normal(id,R*QVector3D(cos(th),sin(th),0.f));
            if (j < thick-1 || j>= layers-thick){
                M->set_vertex_color(id,QVector3D(1.f,1.f,0.f));
                ids.push_back(id);
                pos.push_back(p);
            }
            else
                M->set_vertex_color(id,QVector3D(0.f,0.f,1.f));
            if (j < layers-1){
                uint mni = algo::pos_mod(i+1,resolution);
                M->set_face(id*2,id,mni+flid,(j+1)*resolution+i);
                M->set_face(id*2+1,(j+1)*resolution+mni,(j+1)*resolution+i,mni+flid);
            }
        }
    }
    M->initialize_halfedges(false);
    return {ids,pos};
}

void compute_fairing(algo::geometry::SimpleGLMesh* M) {
    auto borders = build_cylinder(M);
    auto L = M->compute_laplace_beltrami_matrix();
    std::vector<QVector3D> p(M->get_nb_vertices());
    for (uint k = 0;k<3;k++){
        std::vector<float> f(borders.first.size());
        for (uint j = 0;j<borders.first.size();j++)
            f[j] = borders.second[j][k];
        auto X = algo::solve_for_kernel_with_known_variables(L,borders.first,f);
        std::cout << (L*X).norm() << std::endl;
        for (uint j = 0;j<X.rowNum();j++)
            p[j][k] = X(j);
    }
    for (uint j = 0;j<M->get_nb_vertices();j++)
        M->set_vertex_pos(j,p[j]);
    M->compute_normals();
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("fairing");
    auto F = T->add_3D_frame();
    F->set_ambient_light(true);
    F->set_color_mode(mesh_color_mode::given_color);
    auto M = F->set_mesh(constants::layers*constants::resolution,(constants::layers-1)*2*constants::resolution);
    compute_fairing(M);
    w.show();
    return App.exec();
}
