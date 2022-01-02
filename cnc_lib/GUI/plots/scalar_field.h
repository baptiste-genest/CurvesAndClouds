/**
  * @file scalar_field.h defines scalar field type plot (abstract class)
  */
#ifndef SCALAR_FIELD_H
#define SCALAR_FIELD_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "field.h"
#include "core/algo/calculus/calculus.h"

namespace cnc {


class ScalarField : public Field
{
public:
    ScalarField(const algo::calculus::scalar_function_2D&,const range&,const range&);
    ScalarField(const range&,const range&);
    virtual ~ScalarField() {}

    inline  plot_type get_type() const override {
        return scalar_field;
    }

    inline void set_width_resolution(uint w) {
        MAX_WIDTH = w;
        compute_values(frame_info());
    }

    inline void set_height_resolution(uint h) {
        MAX_HEIGHT = h;
        compute_values(frame_info());
    }

    inline void set_resolution(uint v) {
        MAX_HEIGHT = v;MAX_WIDTH = v;
        compute_values(frame_info());
    }

protected:
    void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const override;
    color_policy CP;
    algo::calculus::scalar_function_2D value;
    QPixmap values;
    algo::calculus::nodes z_values;
    range z_range;

    uint MAX_WIDTH = 200;
    uint MAX_HEIGHT = 200;

    inline void plot(frame_draw_object& fdo) override
    {
        fdo.painter.drawPixmap(0,0,fdo.fi.width,fdo.fi.height,values);
    }

    inline virtual void compute_values(const frame_info &) override {}

    inline void compute_value_range(const frame_info &) override {}
};

}

#endif // SCALAR_FIELD_H
