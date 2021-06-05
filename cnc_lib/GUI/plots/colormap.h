/**
  * @file defines colormap plotting (displaying 2D scalar fields by color gradient)
  */
#ifndef COLORMAP_H
#define COLORMAP_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "scalar_field.h"

namespace cnc {

class Colormap : public ScalarField
{
public:
    Colormap(const algo::calculus::scalar_function_2D&,const range&,const range&,color_policy cp);

private:
    void compute_values(const frame_info& fi) override;
    virtual void call_draw_axis(frame_draw_object&) const override;

    ~Colormap() {}
};

}

#endif // COLORMAP_H
