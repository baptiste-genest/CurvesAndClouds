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


class Scalar_field : public Field
{
public:
    Scalar_field(const algo::calculus::scalar_function_2D&,const range&,const range&);
    virtual ~Scalar_field() {}

    inline  plot_type get_type() const override {
        return scalar_field;
    }


protected:
    void init_stat_display(Plot_frame*,Plot_layer*,QVBoxLayout*,const Stat_list&) const override;
    color_policy CP;
    algo::calculus::scalar_function_2D value;
    QPixmap values;
    algo::calculus::nodes z_values;
    range z_range;

    const uint MAX_WIDTH = 500;
    const uint MAX_HEIGHT = 500;

    inline void plot(frame_draw_object& fdo) override
    {
        fdo.painter.drawPixmap(0,0,fdo.fi.width,fdo.fi.height,values);
    }

    inline void compute_values(const frame_info &) override {}

    inline void compute_value_range(const frame_info &) override {}
};

}

#endif // SCALAR_FIELD_H
