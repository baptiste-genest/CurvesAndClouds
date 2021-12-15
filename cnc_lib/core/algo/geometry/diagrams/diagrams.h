#ifndef DIAGRAMS_H
#define DIAGRAMS_H
#include "../combinatorial_complexes.h"
#include "../geometry.h"

namespace cnc {
namespace algo {

namespace topology {
using SimplicialPolygon = std::vector<edge>;
}

namespace geometry {
class Diagram{
public:

    const topology::SimplicialPolygon& operator[](int i) const;

private:
    GeometricContext G;
    std::vector<topology::SimplicialPolygon> cells;
};

}
}
}

#endif // DIAGRAMS_H
