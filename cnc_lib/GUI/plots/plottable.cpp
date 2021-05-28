#include "plottable.h"
using namespace cnc;

Plottable::Plottable()
{
    pg = nullptr;
}

void Plottable::change_label(axis axis_id, const std::string &str)
{
    if (std::regex_match(str, std::regex("[a-zA-Z0-9]+"))){
        if (axis_id == x_axis){
            x_label = str;
        } else {
            y_label = str;
        }
    } else {
        throw Cnc_error("Label can't be void");
    }
}

void Plottable::set_range_reference(Plottable * p)
{
    if (p->range_reference != nullptr)
        set_range_reference(p->range_reference);
    else
        range_reference = p;
}

void Plottable::draw_axis(frame_draw_object& fdo, const std::pair<range, range>& ranges) const
{
    // Scaling down the fontsize
    QFont font = fdo.painter.font() ;
    font.setPointSizeF(font.pointSizeF() * 1.f);
    fdo.painter.setFont(font);

    // nb_bars.fdo.first: number of bars in the x axis | nb_bars.second: number of bars in the y axis
    std::pair<uint, uint> nb_bars;
    nb_bars.first = fdo.fi.width / AXIS_DISTANCE_BETWEEN;
    nb_bars.second = fdo.fi.height / AXIS_DISTANCE_BETWEEN;

    auto Ix = algo::calculus::build_range_mapper({0, (float)nb_bars.first}, ranges.first);
    auto Iy = algo::calculus::build_range_mapper({0, (float)nb_bars.second}, algo::sort(ranges.second));
    //auto Iy = algo::calculus::build_1D_linear_interpolator({0, (float)nb_bars.second}, {ranges.second.first, ranges.second.second});

    QString str;
    for (int x = 0, i = -1; x <= (int) fdo.fi.width; x += AXIS_DISTANCE_BETWEEN, i++){
        fdo.painter.drawLine(x, fdo.fi.height, x, fdo.fi.height - AXIS_SIZE_BAR);
        if (i % AXIS_PRINT_NB_EVERY == 0){
            str = algo_GUI::to_qstring(Ix(i+1), 2);
            fdo.painter.drawText(x - ((float)str.length()/4.f)*algo_GUI::point_to_pixel(font.pointSize()), fdo.fi.height - AXIS_SIZE_BAR, str);
        }
    }
    for (int y = fdo.fi.height, i = -1; y > 0; y -= AXIS_DISTANCE_BETWEEN, i++){
        fdo.painter.drawLine(0, y, AXIS_SIZE_BAR, y);
        if (i % AXIS_PRINT_NB_EVERY == 0){
            str = algo_GUI::to_qstring(Iy(i+1), 2);
            fdo.painter.drawText(0 + AXIS_SIZE_BAR, y + algo_GUI::point_to_pixel(font.pointSize())/2.f, str);
        }
    }

    // print the label's name
    fdo.painter.drawText(AXIS_SIZE_BAR*2, AXIS_SIZE_BAR*2, QString::fromStdString(y_label));
    fdo.painter.drawText(fdo.fi.width - algo_GUI::point_to_pixel(font.pointSize())*(x_label.length()), fdo.fi.height - (AXIS_SIZE_BAR*2), QString::fromStdString(x_label));
}
