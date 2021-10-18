#include "euclidean_geometry.h"

void cnc::euclid::Point::plot(cnc::frame_draw_object &fdo)
{
    range gx_range = get_x_range();
    range gy_range = get_y_range();
    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(gx_range,gy_range, fdo.fi);

    compute_values(fdo.fi);
    fdo.painter.setBrush(plot_color);
    fdo.painter.drawEllipse(M(QPointF(pos(0),pos(1))),radius,radius);
}

void cnc::euclid::Point::compute_values(const cnc::frame_info &)
{
    pos = pos_updater();
}

void cnc::euclid::Point::compute_value_range(const cnc::frame_info &fi)
{
    float eps = 0.1f;
    x_range = {pos(0)-eps,pos(0)+eps};
    y_range = {pos(1)-eps,pos(1)+eps};
}

void cnc::euclid::Line::plot(cnc::frame_draw_object &fdo)
{
    range gx_range = get_x_range();
    range gy_range = get_y_range();
    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(gx_range,gy_range, fdo.fi);

    compute_values(fdo.fi);
    fdo.painter.setPen(QPen(plot_color,width));
    fdo.painter.drawLine(M(QPointF(pos1(0),pos1(1))),M(QPointF(pos2(0),pos2(1))));
}

void cnc::euclid::Line::compute_values(const cnc::frame_info &)
{
    pos1 = pos1_updater();
    pos2 = pos2_updater();
}

void cnc::euclid::Line::compute_value_range(const cnc::frame_info &fi)
{
    x_range = {std::min(pos1(0),pos2(0)),std::max(pos1(0),pos2(0))};
    y_range = {std::min(pos1(1),pos2(1)),std::max(pos1(1),pos2(1))};
}
