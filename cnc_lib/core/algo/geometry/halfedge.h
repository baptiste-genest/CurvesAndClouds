#ifndef HALFEDGE_H
#define HALFEDGE_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
#include "cnc_types.h"
#include "core/algo/lin_alg.h"

namespace cnc {

namespace algo {

namespace geometry {

class Triangle;

struct halfedge
{
    vec* vertex = nullptr;

    halfedge* next = nullptr;
    halfedge* opposite = nullptr;
    uint face_id = 0;
    scalar length;
};


}

}

}

#endif // HALFEDGE_H
