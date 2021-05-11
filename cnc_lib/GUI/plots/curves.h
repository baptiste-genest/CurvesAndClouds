/**
  * @file curves.h defines mother class Curves (can be function plot or 2D curves)
  */
#ifndef CURVES_H
#define CURVES_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plottable.h"
#include "core/algo/calculus/calculus.h"
#include <QColor>

namespace cnc {

class Curves : public Plottable
{
public:
    Curves() {}
    virtual ~Curves() {}

protected:
    uint line_width = 2;
    void plot(frame_draw_object&) override;
    void call_draw_axis(frame_draw_object&) const override;

    algo::calculus::nodes x_values;
    algo::calculus::nodes y_values;
};

}

#endif // CURVES_H
