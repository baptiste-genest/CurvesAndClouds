#include "grid_layer.h"


cnc::GridLayer::GridLayer(cnc::range xr, cnc::range yr, bool display, QWidget *parent)
    : PlotLayer(parent) , x_range(xr) , y_range(yr), display_grid(display)
{
}

void cnc::GridLayer::display_layer(cnc::frame_draw_object &fdo)
{
    if (display_grid){
        float dx = fdo.fi.width/(NB_LINES);
        float dy = fdo.fi.height/(NB_LINES);
        fdo.painter.setPen(QPen(QColorConstants::Gray,1));
        fdo.painter.setOpacity(0.2);
        for (uint j = 0;j<=NB_LINES;j++){
            for (uint i = 0;i<=NB_LINES;i++){
                fdo.painter.drawLine(0,j*dy,fdo.fi.width,j*dy);
                fdo.painter.drawLine(i*dx,0,i*dx,fdo.fi.height);
            }
        }
    }
    fdo.painter.setOpacity(1);
    for (auto plot : plots){
        plot->x_range = x_range;
        plot->y_range = y_range;
    }

    for (uint p = 0;p<plots.size();p++){
        if (plots[p]->dynamic)
            plots[p]->compute_values(fdo.fi);
        fdo.painter.setPen(QPen(plots[p]->plot_color, 2));
        plots[p]->plot(fdo);
    }
    if (plots.size())
        plots[0]->call_draw_axis(fdo);
}
