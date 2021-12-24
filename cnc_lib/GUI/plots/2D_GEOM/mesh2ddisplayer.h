#ifndef MESH2DDISPLAYER_H
#define MESH2DDISPLAYER_H
#include "core/algo/geometry/mesh2.h"
#include "../plottable.h"

namespace cnc{
class Mesh2DDisplayer : public Plottable {
public:
    Mesh2DDisplayer(const algo::geometry::Mesh2& M);
    inline plot_type get_type() const override{ return plot_type::d2_curve;}

private:
    void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const override{}
    virtual void call_draw_axis(frame_draw_object&) const override{}

    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override{}
    virtual void compute_value_range(const frame_info& fi) override;
    const algo::geometry::Mesh2* M;

};
}

#endif // MESH2DDISPLAYER_H
