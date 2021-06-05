#ifndef PLOT_GROUP_H
#define PLOT_GROUP_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
#include "plot_layer.h"
#include "plot_frame.h"

namespace cnc {

class PlotGroup : public PlotLayer {
public:
    PlotGroup(QWidget* parent);
private:
    void display_layer(frame_draw_object& fdo) override;

};

}

#endif // PLOT_GROUP_H
