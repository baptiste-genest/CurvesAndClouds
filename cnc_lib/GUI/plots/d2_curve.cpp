#include "d2_curve.h"
using namespace cnc;

D2_curve::D2_curve(const algo::calculus::nodes &X, const algo::calculus::nodes &Y){
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

void D2_curve::compute_value_range(const frame_info &)
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

