#ifndef SDF_H
#define SDF_H
#include "cnc_types.h"
#include "geometry.h"

namespace cnc {
namespace algo {
namespace geometry {

struct SDF
{
    static scalar toSegment(const vec& x,const vec& a,const vec& b);
};

}
}
}

#endif // SDF_H
