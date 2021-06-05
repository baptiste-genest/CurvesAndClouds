#include "trajectory.h"


cnc::Trajectory::Trajectory(const cnc::algo::calculus::parametrization_1D &p, cnc::scalar t0, cnc::scalar dt) : p(p),t(t0),dt(dt)
{
    Plottable::dynamic = true;
    Trajectory::compute_values(frame_info());
}

void cnc::Trajectory::compute_values(const cnc::frame_info &)
{
    vec x = p(t);
    t += dt;
    x_values.push_back(x(0));
    y_values.push_back(x(1));
}

void cnc::Trajectory::plot(cnc::frame_draw_object& fdo)
{
    D2Curve::plot(fdo);
    auto X = algo::calculus::build_range_mapper(get_x_range(),{0,fdo.fi.width});
    auto Y = algo::calculus::build_range_mapper(get_y_range(),{0,fdo.fi.height});
    fdo.painter.drawEllipse(X(x_values.back()),fdo.fi.height - Y(y_values.back()),3,3);
}
