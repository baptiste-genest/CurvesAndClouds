#include "mesh2ddisplayer.h"

cnc::Mesh2DDisplayer::Mesh2DDisplayer(const cnc::algo::geometry::Mesh2 &mesh) : M(&mesh)
{

}

void cnc::Mesh2DDisplayer::plot(cnc::frame_draw_object &fdo)
{
    algo_GUI::mapping Mapper = algo_GUI::gen_mapping_value_to_frame(x_range,y_range, fdo.fi);
    const auto& G = *M->G;
    fdo.painter.setPen(QPen(Plottable::plot_color,2));
    for (const auto& e : M->boundary){
        auto P1 =Mapper(algo_GUI::vec_to_QPointF(G(e[0])));
        auto P2 =Mapper(algo_GUI::vec_to_QPointF(G(e[1])));
        fdo.painter.drawLine(P1,P2);
    }
    fdo.painter.setPen(QPen(Plottable::plot_color,1));
    for (const auto& e : M->interiorEdges()){
        auto P1 =Mapper(algo_GUI::vec_to_QPointF(G(e[0])));
        auto P2 =Mapper(algo_GUI::vec_to_QPointF(G(e[1])));
        fdo.painter.drawLine(P1,P2);
    }
    for (const auto& v : M->V){
        auto P =Mapper(algo_GUI::vec_to_QPointF(G(v)));
        fdo.painter.drawEllipse(P,3,3);
    }
}

void cnc::Mesh2DDisplayer::compute_value_range(const cnc::frame_info &)
{
    uint N = M->V.size();
    std::vector<scalar> coord(N);
    for (int i = 0;i<(int)N;i++)
        coord[i] = (*M->G)(i)(0);
    x_range = algo::get_min_max_range(coord);
    for (int i = 0;i<(int)N;i++)
        coord[i] = (*M->G)(i)(1);
    y_range = algo::get_min_max_range(coord);
}
