#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include "mesh2.h"
#include <stack>

namespace cnc {

namespace algo {

namespace geometry {

namespace convex_hull_routines {

NormalMap compute_orientation(const GeometricContext&,const faces&);
scalar slope(const vec& A,const vec& B);
face get_first_facet(const GeometricContext& G);

}

Mesh2 giftWrapping(const GeometricContext& C);

}

}

}


#endif // CONVEX_HULL_H
