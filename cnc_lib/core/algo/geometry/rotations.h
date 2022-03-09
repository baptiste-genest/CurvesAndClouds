#ifndef ROTATIONS_H
#define ROTATIONS_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "../linear_algebra/lin_alg.h"

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

namespace cnc {

namespace algo{

namespace geometry {

mat Rx(scalar th);
mat Ry(scalar th);
mat Rz(scalar th);

inline scalar toRadians(scalar deg) {
    return deg*M_PI/180.;
}

namespace degrees {
inline mat Rx(scalar th){
    return geometry::Rx(toRadians(th));
}
inline mat Ry(scalar th){
    return geometry::Ry(toRadians(th));
}
inline mat Rz(scalar th){
    return geometry::Rz(toRadians(th));
}
}

}
}

}

#endif // ROTATIONS_H
