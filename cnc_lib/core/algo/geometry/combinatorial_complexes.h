#ifndef COMBINATORIAL_COMPLEXES_H
#define COMBINATORIAL_COMPLEXES_H
#include "cnc_types.h"
#include <set>
#include <vector>
#include <array>
#include "../lin_alg.h"
#include "../matrix_functions.h"
#include "../statistics/statistics.h"

namespace cnc{

namespace algo {

namespace topo {

using vertex = int;
using vertices = std::set<vertex>;
using edge = std::array<vertex,2>;
using face = std::array<edge,3>;

bool operator==(const edge& a,const edge& b);
bool operator==(const face& a,const face& b);

vertices get_vertices(const face& F);
edge get_common_edge(const face& F,vertex O);
vertex get_other(const face& F,const edge& e);

}
}
}

#endif // COMBINATORIAL_COMPLEXES_H
