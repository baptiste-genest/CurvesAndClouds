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

class LevelCurve : public ScalarField
{
public:
    LevelCurve(const algo::calculus::scalar_function_2D&,const range&,const range&,uint = 10);
    LevelCurve(const algo::calculus::scalar_function_2D&,const range&,const range&,float = 0.f);

private:
    void compute_values(const frame_info& fi) override;
    virtual void call_draw_axis(frame_draw_object&) const override;
    uint nb_slices;
    ~LevelCurve() {}
};

}

#endif // LEVEL_CURVE_H
