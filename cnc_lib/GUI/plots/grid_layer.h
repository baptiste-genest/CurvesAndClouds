#ifndef GRID_LAYER_H
#define GRID_LAYER_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plot_layer.h"

namespace cnc {

class GridLayer : public PlotLayer
{
public:
    GridLayer(range xr,range yr,bool display,QWidget* parent);

protected:
    virtual void display_layer(frame_draw_object& fdo) override;

private:
    range x_range,y_range;
    bool display_grid;

    static const uint NB_LINES = 20;
};

}

#endif // GRID_LAYER_H
