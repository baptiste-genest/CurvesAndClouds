#ifndef ROTATIONS_H
#define ROTATIONS_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "../lin_alg.h"


namespace cnc {

namespace algo{

namespace geometry {

mat Rx(scalar th);
mat Ry(scalar th);
mat Rz(scalar th);

}
}

}

#endif // ROTATIONS_H
