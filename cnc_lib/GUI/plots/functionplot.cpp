#include "functionplot.h"
using namespace cnc;


FunctionPlot::FunctionPlot(const algo::calculus::scalar_function_1D &v, const range &x,plot_axis_policy _fpp) : value(v)
{
    illustrative = false;
    pap = _fpp;
    if (x.first > x.second)
        throw Cnc_error("plot interval for a to b must have a < b");
    x_range = x;
    x_values = algo::calculus::get_lin_space(x_range.first,x_range.second,NB_X_SAMPLES);
    y_values.resize(NB_X_SAMPLES);
    FunctionPlot::compute_values(frame_info());
}


void FunctionPlot::compute_value_range(const frame_info &)
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

void FunctionPlot::compute_values(const frame_info&)
{
    for (uint i = 0;i<x_values.size();i++)
        y_values[i] = value(x_values[i]);
}
