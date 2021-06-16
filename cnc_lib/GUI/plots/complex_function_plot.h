#ifndef COMPLEX_FUNCTION_PLOT_H
#define COMPLEX_FUNCTION_PLOT_H

#if defined(CNC_LIB)
#define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "scalar_field.h"
#include "core/algo/calculus/calculus.h"
#include "GUI/ColorWheel.h"

namespace cnc {

class ComplexFunctionPlot : public ScalarField
{
public:
    ComplexFunctionPlot(const algo::calculus::complex_function& f,range re,range im);

private:
    algo::calculus::complex_function f;

    void compute_values(const frame_info&) override;
    virtual void call_draw_axis(frame_draw_object&) const override;


};

}

#endif // COMPLEX_FUNCTION_PLOT_H
