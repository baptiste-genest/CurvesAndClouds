#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::geometry;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("3D plotting");

    MeshDisplayer* F3D = T->add_3D_frame();
    F3D->load_mesh(stanford_bunny);
    F3D->set_color_mode(interpolate_value);
    auto mesh = F3D->get_mesh();

    std::vector<QVector3D> face_normals(mesh->get_nb_faces());
    for (uint f = 0;f<mesh->get_nb_faces();f++)
        face_normals[f] = mesh->get_face_normal(f);
    auto div = mesh->integrated_divergence_per_vertex(face_normals)*1e9;
    std::cout << div.norm_inf() << std::endl;
    mesh->set_vertex_values(div);

    w.show();
    return App.exec();
}
