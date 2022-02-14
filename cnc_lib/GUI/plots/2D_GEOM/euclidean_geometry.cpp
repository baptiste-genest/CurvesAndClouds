#include "euclidean_geometry.h"


void cnc::euclid::Point::draw(frame_draw_object& fdo,range rx,range ry)
{
    update_values();

    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(rx,ry, fdo.fi);

    fdo.painter.drawEllipse(M(QPointF(pos(0),pos(1))),radius,radius);
}

void cnc::euclid::Point::update_values()
{
    pos = pos_updater();
}

void cnc::euclid::Segment::draw(frame_draw_object& fdo,range rx,range ry)
{
    update_values();
    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(rx,ry,fdo.fi);

    fdo.painter.setPen(QPen(color,width));
    fdo.painter.drawLine(M(QPointF(pos1(0),pos1(1))),M(QPointF(pos2(0),pos2(1))));
}

void cnc::euclid::Segment::update_values()
{
    pos1 = point1->get_pos();
    pos2 = point2->get_pos();
}

void cnc::euclid::Circle::draw(cnc::frame_draw_object &fdo, cnc::range rx, cnc::range ry)
{
    auto tmp = fdo.painter.brush();
    update_values();
    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(rx,ry, fdo.fi);
    auto c(M(QPointF(pos(0),pos(1))));
    auto p = M(QPointF(pos(0)+radius,pos(1)));
    auto r = p.x()-c.x();
    fdo.painter.setBrush(Qt::NoBrush);
    fdo.painter.drawEllipse(c,r,r);
    fdo.painter.setBrush(tmp);
}

void cnc::euclid::Circle::update_values()
{
    pos = pos_updater();
    radius = rad_updater();
}
