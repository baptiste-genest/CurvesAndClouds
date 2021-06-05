#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "d2_curve.h"
#include "core/algo/calculus/calculus.h"

namespace cnc {

class Trajectory : public D2Curve
{
public:
    Trajectory(const algo::calculus::parametrization_1D& p,scalar t0,scalar dt);
private:
    algo::calculus::parametrization_1D p;
    scalar t;
    scalar dt;

    void compute_values(const frame_info&) override;
    void plot(frame_draw_object&) override;
};

}
#endif // TRAJECTORY_H
