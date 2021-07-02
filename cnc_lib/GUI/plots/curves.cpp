#include "curves.h"
using namespace cnc;

void Curves::plot(frame_draw_object& fdo)
{
    algo_GUI::mapping M = algo_GUI::gen_mapping_value_to_frame(get_x_range(),algo::sort(get_y_range()),fdo.fi);

    for (uint i = 0;i<x_values.size()-1;i++){
        //set_segment_color(fdo,i);
        fdo.painter.drawLine(M(QPointF(x_values[i],y_values[i])),M(QPointF(x_values[i+1],y_values[i+1])));
        //std::cout << i << std::endl;
    }
}

void Curves::call_draw_axis(frame_draw_object& fdo) const {
    fdo.painter.setPen(QPen(Qt::black, 1));
    Plottable::draw_axis(fdo, {x_range,y_range});
}
