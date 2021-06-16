#ifndef FIELD_H
#define FIELD_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plottable.h"

namespace cnc {

class Field : public Plottable
{
public:
    Field();

    void set_axis_policy(plot_axis_policy) = delete;
    void set_range_reference(Plottable*) = delete;
    void fix_plot_in_rect(float,float,float,float) = delete;
    virtual ~Field() {}
};

}

#endif // FIELD_H
