#include "diagramplotter.h"

cnc::DiagramPlotter::DiagramPlotter(const algo::geometry::Diagram &Dp) : D(Dp)
{
    RV = {-D.RV,D.RV};

}

void cnc::DiagramPlotter::plot(cnc::frame_draw_object &fdo)
{
    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(x_range,y_range, fdo.fi);
    fdo.painter.setPen(QColorConstants::Black);
    for (const auto& e : D.edges){
        auto P1 =M(algo_GUI::vec_to_QPointF(D.G(e[0])));
        auto P2 =M(algo_GUI::vec_to_QPointF(D.G(e[1])));
        fdo.painter.drawLine(P1,P2);
    }
}

void cnc::DiagramPlotter::compute_value_range(const cnc::frame_info &fi)
{
    x_range = RV;
    y_range = RV;
}
