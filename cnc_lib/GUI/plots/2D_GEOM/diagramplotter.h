#ifndef DIAGRAMPLOTTER_H
#define DIAGRAMPLOTTER_H

#include "../../../core/algo/geometry/diagrams/diagrams.h"
#include "../plottable.h"

namespace cnc {

class DiagramPlotter : public Plottable
{
public:
    DiagramPlotter(const algo::geometry::Diagram& D);
    inline plot_type get_type() const override{ return plot_type::d2_curve;}

private:
    range RV;
    const algo::geometry::Diagram& D;
    void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const override{}
    virtual void call_draw_axis(frame_draw_object&) const override{}

    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override{}
    virtual void compute_value_range(const frame_info& fi) override;
};

}

#endif // DIAGRAMPLOTTER_H
