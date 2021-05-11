/**
  *	@file vector_field.h defines vector field plotting
  */
#ifndef VECTOR_FIELD_H
#define VECTOR_FIELD_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "field.h"
#include "core/algo/calculus/calculus.h"

namespace cnc {

class Vector_field : public Field
{
public:
    Vector_field(const algo::calculus::vector_function_2D&,const range&,const range&,uint);

    virtual ~Vector_field() {}

    inline  plot_type get_type() const override {
        return vector_field;
    }

protected:
    void init_stat_display(Plot_frame*,Plot_layer*,QVBoxLayout*,const Stat_list&) const override;

    algo::calculus::vector_function_2D value;
    uint nb_vector_per_line;
    range norm_range;
    std::vector<cnc::vec> samples;

    void plot(frame_draw_object& fdo) override;

    inline void compute_values(const frame_info &) override {}

    inline void compute_value_range(const frame_info &) override {}

    inline void call_draw_axis(frame_draw_object &fdo) const override
    {
        fdo.painter.setPen(QPen(Qt::black, 1));
        Plottable::draw_axis(fdo, {x_range,y_range});
    }
};

}

#endif // VECTOR_FIELD_H
