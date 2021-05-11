#include "function_plot.h"
using namespace cnc;


Function_plot::Function_plot(const algo::calculus::scalar_function_1D &v, const range &x,plot_axis_policy _fpp) : value(v)
{
    illustrative = false;
    pap = _fpp;
    if (x.first > x.second)
        throw Cnc_error("plot interval for a to b must have a < b");
    x_range = x;
}


void Function_plot::compute_value_range(const frame_info &)
{
    switch (pap){
            case (min_max_range):
                y_range = algo::get_min_max_range(y_values);
                return;
            case (same_than_x_range):
                y_range = x_range;
                return;
            case (given_range):
                return;
            default:
                return;
    }
}

void Function_plot::compute_values(const frame_info& fi)
{
    x_values = algo::calculus::get_lin_space(x_range.first,x_range.second,(int)fi.width);
    y_values.resize(x_values.size());
    for (uint i = 0;i<x_values.size();i++)
        y_values[i] = value(x_values[i]);
}
