/**
  * @file function_plot.h defines function plotting
  */
#ifndef FUNCTIONPLOT_H
#define FUNCTIONPLOT_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "curves.h"
#include "core/algo/calculus/calculus.h"
#include "core/algo/calculus/integration_scheme.h"
#include "GUI/algo/algo_gui.h"
#include "plot_policy.h"

namespace cnc {

class FunctionPlot : public Curves
{
public:
    FunctionPlot(const algo::calculus::scalar_function_1D& value,const range& x_range,plot_axis_policy);
    ~FunctionPlot() {}

    inline  plot_type get_type() const override {
        return function_plot;
    }

protected:
    void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const override;

private:

    void compute_value_range(const frame_info& fi) override;
    void compute_values(const frame_info& fi) override;

    algo::calculus::scalar_function_1D value;

    range get_y_range(const algo::calculus::nodes&) const;

    const static int NB_X_SAMPLES = 700;

};

}

#endif // FUNCTIONPLOT_H
