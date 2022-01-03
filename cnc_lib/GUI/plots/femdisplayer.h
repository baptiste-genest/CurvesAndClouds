#ifndef FEMDISPLAYER_H
#define FEMDISPLAYER_H

#include "../../core/algo/calculus/finite_elements/finite_elements.h"
#include "../../core/algo/geometry/diagrams/diagrams.h"
#include "plottable.h"

namespace cnc {

class FEMDisplayer : public Plottable
{
public:
    FEMDisplayer(const algo::FEM::FiniteElementSolver& F);
    inline plot_type get_type() const override{ return plot_type::d2_curve;}

private:
    void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const override{}
    virtual void call_draw_axis(frame_draw_object&) const override{}

    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override{}
    virtual void compute_value_range(const frame_info&) override;
    const algo::FEM::FiniteElementSolver& solver;
    //algo::geometry::Diagram D;
    std::vector<QColor> centroids_color;
    std::vector<QPolygonF> cells;
};

}

#endif // FEMDISPLAYER_H
