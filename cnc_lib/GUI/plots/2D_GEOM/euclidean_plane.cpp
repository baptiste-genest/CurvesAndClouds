#include "euclidean_plane.h"

cnc::euclid::EuclideanPlane::EuclideanPlane(cnc::range x, cnc::range y)
{
    x_range = x;
    y_range = y;
    fixed_range = true;
    dynamic = true;
}

void cnc::euclid::EuclideanPlane::plot(cnc::frame_draw_object &fdo)
{
    for (EuclideanPrimitive* p : m_objects)
        p->draw(fdo);
}

void cnc::euclid::EuclideanPlane::compute_values(const cnc::frame_info &)
{
    for (EuclideanPrimitive* p : m_objects)
        if (p->is_dynamic())
            p->update_values();
}

void cnc::euclid::EuclideanPlane::compute_value_range(const cnc::frame_info &fi)
{
    if (fixed_range)
        return;
    x_range = range{0,0};
    y_range = range{0,0};
    for (EuclideanPrimitive* p : m_objects){
        auto rx = p->get_x_range();
        auto ry = p->get_y_range();

        algo::extend_range(x_range,rx);
        algo::extend_range(y_range,ry);
    }

}
