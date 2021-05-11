/**
  * @file function_plot.h defines function plotting
  */
#ifndef FUNCTION_PLOT_H
#define FUNCTION_PLOT_H
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

class Function_plot : public Curves
{
public:
    Function_plot(const algo::calculus::scalar_function_1D& value,const range& x_range,plot_axis_policy);
    ~Function_plot() {}

    inline  plot_type get_type() const override {
        return function_plot;
    }

protected:
    void init_stat_display(Plot_frame*,Plot_layer*,QVBoxLayout*,const Stat_list&) const override;

private:

    void compute_value_range(const frame_info& fi) override;
    void compute_values(const frame_info& fi) override;

    algo::calculus::scalar_function_1D value;

    range get_y_range(const algo::calculus::nodes&) const;

};

}

#endif // FUNCTION_PLOT_H
