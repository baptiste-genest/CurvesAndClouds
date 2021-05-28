#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::calculus;
using namespace cnc::algo::geometry;

void generate_borders(QuadtreeMesher& QM,uint N){
    for (uint k = 0;k<N;k++)
        QM.add_to_chain(vec({std::cos(2*M_PI*k/N),std::sin(2*M_PI*k/N)}));
    QM.complete_chain();

    for (uint k = 0;k<N;k++)
        QM.add_to_chain(vec({std::cos(2*M_PI*k/N),std::sin(2*M_PI*k/N)})*.3);
    QM.complete_chain();

}

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    Plot_window w; w.resize(500,500);
    Plot_tab* T = w.add_tab("Quadtree meshing");

    range R({-1.2,1.2});
    uint N = 20;
    QuadtreeMesher QM;
    generate_borders(QM,N);

    QM.build_mesh();
    {
        auto L = QM.get_edges();
        auto layer = T->add_frame()->add_layer();
        for (const auto& l : L)
            layer->new_2D_curve(l)->fix_plot_range(R,R);
    }
    {
        auto layer = T->add_frame()->add_layer();
        auto M = QM.get_mesh();
        for (const Triangle& t : M){
            layer->new_2D_curve(t.get_edges())->fix_plot_range(R,R);
        }
    }

    w.show();
    return a.exec();
}

/*
    {
        cloud C; C.points = QM.mesh_points;
        layer->new_point_cloud(C)->fix_plot_range(R,R);
    }
    {
        auto L = QM.get_quadtree()->get_lines();
        for (const auto& l : L)
            layer->new_2D_curve(l)->fix_plot_range(R,R);
    }

scalar_function_2D get_k_barycentric(uint k,const Triangle& T){
    return [k,T] (float x,float y) {
        return T.get_barycentric(vec({x,y}))[k];
    };
}
    for (uint k = 0;k<3;k++){
    Plot_layer* L = F->add_layer();
    L->new_colormap(get_k_barycentric(k,A),R,R,from_zero);
    L->new_point_cloud(cv)->fix_plot_range(R,R);
    }


 */
