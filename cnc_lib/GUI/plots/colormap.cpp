#include "colormap.h"
using namespace cnc;

Colormap::Colormap(const algo::calculus::scalar_function_2D &f, const range & x, const range &y,color_policy cp) : ScalarField(f,x,y)
{
    CP = cp;
    Colormap::compute_values(frame_info());
}

void Colormap::compute_values(const frame_info &)
{
    QImage tmp(MAX_HEIGHT,MAX_WIDTH,QImage::Format_RGBA64);
    float mid_val = (z_range.first + z_range.second)*0.5f;

    auto X = algo::calculus::build_range_mapper({0,MAX_WIDTH},x_range,true);
    auto Y = algo::calculus::build_range_mapper({0,MAX_HEIGHT},y_range,true);

    algo::calculus::scalar_function_1D R,B;
    switch (CP){
    case min_max:
        R = algo::calculus::build_range_mapper({mid_val,z_range.second},{0,255},true);
        B = algo::calculus::build_range_mapper({z_range.first,mid_val},{255,0},true);
        break;
    case from_zero:
        if (std::abs(z_range.second) < 0.001f)
            R = [] (float) {return 0.f;};
        else
            R = algo::calculus::build_range_mapper({0,std::max(scalar(0),z_range.second)},{0,255},true);
        if (std::abs(z_range.first) < 0.001f)
            B = [] (float) {return 0.f;};
        else
            B = algo::calculus::build_range_mapper({std::min(scalar(0),z_range.first),0},{255,0},true);
        break;
    }

    for (uint j = 0;j< MAX_HEIGHT;j++){
        for (uint i = 0;i<MAX_WIDTH;i++){
            float z = value(X(i),Y(j));
            auto C = QColor::fromRgb(R(z),0,B(z));
            tmp.setPixelColor(i,MAX_HEIGHT - 1 - j,C);
        }
    }

    values = QPixmap::fromImage(tmp);
}


void Colormap::call_draw_axis(frame_draw_object &fdo) const
{
    fdo.painter.setPen(QPen(Qt::white, 1));
    Plottable::draw_axis(fdo, {x_range,y_range});
}
