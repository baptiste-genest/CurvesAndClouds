#include "plot_group.h"
using namespace cnc;

Plot_group *Plot_frame::add_plot_group()
{
    Plot_group* g = new Plot_group(this);
    layers.push_back((Plot_layer*)g);
    return g;
}


Plot_group::Plot_group(QWidget *parent) : Plot_layer(parent)
{

}

void Plot_group::display_layer(frame_draw_object &fdo)
{
    range max_frame_x = {MAXFLOAT,-MAXFLOAT};
    range max_frame_y = {MAXFLOAT,-MAXFLOAT};
    std::vector<uint> free_pl,dependant;
    for (uint p = 0;p<plots.size();p++){
        plots[p]->compute_values(fdo.fi);
        if (plots[p]->range_reference == nullptr)
            free_pl.push_back(p);
        else
            dependant.push_back(p);
    }
    for (uint f : free_pl)
        plots[f]->compute_value_range(fdo.fi);

    for (uint d : dependant){
        plots[d]->x_range = plots[d]->get_x_range();
        plots[d]->y_range = plots[d]->get_y_range();
    }

    for (uint p = 0;p<plots.size();p++){
        algo::extend_range(max_frame_x,plots[p]->get_x_range());
        algo::extend_range(max_frame_y,plots[p]->get_y_range());
    }

    for (uint p = 0;p<plots.size();p++){
        plots[p]->x_range = max_frame_x;
        plots[p]->y_range = max_frame_y;
        fdo.painter.setPen(QPen(plots[p]->plot_color, 2));
        plots[p]->plot(fdo);
    }
    if (plots.size())
        plots[0]->call_draw_axis(fdo);
}
