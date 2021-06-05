#include "scalar_field.h"
using namespace cnc;

ScalarField::ScalarField(const algo::calculus::scalar_function_2D &f, const range & x, const range &y) : value(f)
{
    Plottable::x_range = x;
    Plottable::y_range = y;

    algo::calculus::nodes x_values = algo::calculus::get_lin_space(x_range.first,x_range.second,(int)MAX_WIDTH);
    algo::calculus::nodes y_values = algo::calculus::get_lin_space(y_range.first,y_range.second,(int)MAX_HEIGHT);
    const uint nx = x_values.size();
    const uint ny = y_values.size();

    z_values.resize(nx*ny);

    for (uint j = 0;j<y_values.size();j++)
        for (uint i = 0;i<x_values.size();i++)
            z_values[j*nx+i] = ScalarField::value(x_values[i],y_values[j]);

    z_range = algo::get_min_max_range(z_values);
}
