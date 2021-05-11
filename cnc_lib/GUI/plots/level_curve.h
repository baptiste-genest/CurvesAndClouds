/**
  * @file level_curve.h defines level curve plots
  */
#ifndef LEVEL_CURVE_H
#define LEVEL_CURVE_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "scalar_field.h"
#include "core/algo/calculus/vector_calculus.h"

namespace cnc{

class Level_curve : public Scalar_field
{
public:
    Level_curve(const algo::calculus::scalar_function_2D&,const range&,const range&,uint = 10);
    Level_curve(const algo::calculus::scalar_function_2D&,const range&,const range&,float = 0.f);

private:
    virtual void call_draw_axis(frame_draw_object&) const override;
    ~Level_curve() {}
};

}

#endif // LEVEL_CURVE_H
