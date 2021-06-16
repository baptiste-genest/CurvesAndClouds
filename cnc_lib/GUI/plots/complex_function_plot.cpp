#include "complex_function_plot.h"

cnc::ComplexFunctionPlot::ComplexFunctionPlot(const cnc::algo::calculus::complex_function &_f, cnc::range re, cnc::range im)
    : ScalarField(re,im) , f(_f)
{
    ComplexFunctionPlot::compute_values(frame_info());
}

void cnc::ComplexFunctionPlot::compute_values(const cnc::frame_info &)
{
    QImage tmp(MAX_HEIGHT,MAX_WIDTH,QImage::Format_RGBA64);

    auto X = algo::calculus::build_range_mapper({0,MAX_WIDTH},x_range);
    auto Y = algo::calculus::build_range_mapper({0,MAX_HEIGHT},y_range);
    for (uint j = 0;j< MAX_HEIGHT;j++){
        for (uint i = 0;i<MAX_WIDTH;i++){
            complex_scalar z = f(complex_scalar(X(i),Y(j)));
            tmp.setPixelColor(i,MAX_HEIGHT - 1 - j,get_RGB_by_angle(z,true));
        }
    }

    values = QPixmap::fromImage(tmp);
}

void cnc::ComplexFunctionPlot::call_draw_axis(cnc::frame_draw_object & fdo) const
{
    fdo.painter.setPen(QPen(Qt::white, 1));
    Plottable::draw_axis(fdo, {x_range,y_range});
}
