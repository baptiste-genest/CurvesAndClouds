#include "d2_curve.h"
using namespace cnc;

D2Curve::D2Curve(const algo::calculus::nodes &X, const algo::calculus::nodes &Y,bool color): color_by_order(color){
    if (X.size() == 0)
        throw Cnc_error("can't build 2D curve without X coords");
    if (Y.size() == 0)
        throw Cnc_error("can't build 2D curve without Y coords");
    x_values = X;
    y_values = Y;
    if (X.size() != Y.size())
        throw Cnc_error("must have as much X than Y coords");
    pap = same_than_x_range;
}

void D2Curve::compute_value_range(const frame_info &)
{
    switch (pap){
            case (min_max_range):
                x_range = algo::get_min_max_range(x_values);
                y_range = algo::get_min_max_range(y_values);
                algo::scale_range(x_range,1.2f);
                algo::scale_range(y_range,1.2f);
                break;
            case (same_than_x_range):
                x_range = algo::get_min_max_range(x_values);
                algo::scale_range(x_range,1.2f);
                y_range = x_range;
                break;
            default:
                return;
    }
}

void D2Curve::set_segment_color(frame_draw_object& fdo, uint i) const
{
    if (!color_by_order)
        return;
    auto R = algo::calculus::build_range_mapper({0,x_values.size()},{0,255});
    auto B = algo::calculus::build_range_mapper({0,x_values.size()},{255,0});
    fdo.painter.setPen(QPen(QColor::fromRgb(R(i),0,B(i)),2));
}

