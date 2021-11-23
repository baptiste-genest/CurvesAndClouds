#include "boolean_field.h"

cnc::BooleanField::BooleanField(const cnc::algo::calculus::property_2D &P, cnc::range x, cnc::range y,const QColor& c) : P(P){
    Plottable::x_range = x;
    Plottable::y_range = y;

    own_color = c;
    BooleanField::compute_values(frame_info());
}

void cnc::BooleanField::compute_values(const cnc::frame_info &)
{
    static const uint MAX_WIDTH = 200;
    static const uint MAX_HEIGHT = 200;
    QImage tmp(MAX_HEIGHT,MAX_WIDTH,QImage::Format_RGBA64);

    auto X = algo::calculus::build_range_mapper({0,MAX_WIDTH},x_range);
    auto Y = algo::calculus::build_range_mapper({0,MAX_HEIGHT},y_range);

    auto C = own_color;
    C.setAlpha(122);

    uint count = 0;
    for (uint j = 0;j< MAX_HEIGHT;j++){
        for (uint i = 0;i<MAX_WIDTH;i++){
            bool z = P(X(i),Y(j));
            if (z){
                tmp.setPixelColor(i,MAX_HEIGHT - 1 - j,C);
                count++;
            }
            else
                tmp.setPixelColor(i,MAX_HEIGHT - 1 - j,QColor(0,0,0,0));
        }
    }
    //std::cout << "nb of points: " << count << std::endl;

    img = QPixmap::fromImage(tmp);
}
