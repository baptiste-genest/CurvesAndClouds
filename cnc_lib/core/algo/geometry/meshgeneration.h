#ifndef MESHGENERATION_H
#define MESHGENERATION_H

#include "mesh2.h"
#include "convex_hull.h"

namespace cnc {
namespace algo {
namespace geometry {

struct mesh_generation {
static Mesh2 Delaunay(const cloud& X,bool proj2D = true);
};
}
}
}

#endif // MESHGENERATION_H
