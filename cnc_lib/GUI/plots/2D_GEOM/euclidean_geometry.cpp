#include "euclidean_geometry.h"


void cnc::euclid::Point::draw(frame_draw_object& fdo)
{
    update_values();

    range gx_range = get_x_range();
    range gy_range = get_y_range();
    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(gx_range,gy_range, fdo.fi);

    fdo.painter.drawEllipse(M(QPointF(pos(0),pos(1))),radius,radius);
}

void cnc::euclid::Point::update_values()
{
    pos = pos_updater();
}

void cnc::euclid::Segment::draw(frame_draw_object& fdo)
{
    update_values();
    range gx_range = get_x_range();
    range gy_range = get_y_range();
    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(gx_range,gy_range, fdo.fi);

    fdo.painter.setPen(QPen(color,width));
    fdo.painter.drawLine(M(QPointF(pos1(0),pos1(1))),M(QPointF(pos2(0),pos2(1))));
}

void cnc::euclid::Segment::update_values()
{
    pos1 = point1->get_pos();
    pos2 = point2->get_pos();
}
