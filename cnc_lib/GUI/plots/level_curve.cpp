#include "level_curve.h"
using namespace cnc;

Level_curve::Level_curve(const algo::calculus::scalar_function_2D &f, const range &x, const range &y,uint nb_slices) : Scalar_field(f,x,y)
{
    QImage tmp(MAX_HEIGHT,MAX_WIDTH,QImage::Format_RGBA64);
    float mid_val = (z_range.first + z_range.second)*0.5f;
    auto levels = algo::calculus::get_lin_space(z_range.first,z_range.second,(int)nb_slices);

    auto X = algo::calculus::build_range_mapper({0,MAX_WIDTH},x_range);
    auto Y = algo::calculus::build_range_mapper({0,MAX_HEIGHT},y_range);

    auto R = algo::calculus::build_range_mapper({mid_val,z_range.second},{0,255});
    auto B = algo::calculus::build_range_mapper({z_range.first,mid_val},{255,0});

    std::vector<float> dist_to_levels(nb_slices);
    float thickness = 0.2f*(z_range.second - z_range.first)/nb_slices;
    auto grad = algo::vector_calculus::build_gradient(Scalar_field::value);

    for (uint j = 0;j< MAX_HEIGHT;j++){
        for (uint i = 0;i<MAX_WIDTH;i++){
            float z = f(X(i),Y(j));
            float grad_norm = grad(X(i),Y(j)).norm();
            for (uint k = 0;k<nb_slices;k++)
                dist_to_levels[k] = std::abs(z-levels[k]);
            auto m = std::min_element(dist_to_levels.begin(),dist_to_levels.end());
            uint id = std::distance(dist_to_levels.begin(),m);
            if (*(m) < thickness*grad_norm){
                auto C = QColor::fromRgb(R(levels[id]),0,B(levels[id]));
                tmp.setPixelColor(i,MAX_HEIGHT - 1 - j,C);
            }
        }
    }

    values = QPixmap::fromImage(tmp);
}

Level_curve::Level_curve(const algo::calculus::scalar_function_2D & f, const range & x, const range & y, float level) : Scalar_field(f,x,y)
{
    QImage tmp(MAX_HEIGHT,MAX_WIDTH,QImage::Format_RGBA64);

    auto X = algo::calculus::build_range_mapper({0,MAX_WIDTH},x_range);
    auto Y = algo::calculus::build_range_mapper({0,MAX_HEIGHT},y_range);

    auto grad = algo::vector_calculus::build_gradient(f);
    static const float thickness = 0.01f;

    for (uint j = 0;j< MAX_HEIGHT;j++){
        for (uint i = 0;i<MAX_WIDTH;i++){
            float z = f(X(i),Y(j));
            float grad_norm = grad(X(i),Y(j)).norm();
            if (std::abs(z - level) < thickness*grad_norm){
                tmp.setPixelColor(i,MAX_HEIGHT-j-1,QColorConstants::Black);
            }
        }
    }

    values = QPixmap::fromImage(tmp);
}

void Level_curve::call_draw_axis(frame_draw_object &fdo) const
{
    fdo.painter.setPen(QPen(Qt::black, 1));
    Plottable::draw_axis(fdo, {x_range,y_range});

}
