#ifndef DIAGRAMS_H
#define DIAGRAMS_H
#include "../combinatorial_complexes.h"
#include "../geometry.h"
#include "../mesh2.h"
#include "../convex_hull.h"
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


namespace cnc {

class DiagramPlotter;

namespace algo {
namespace topology {
using SimplicialPolygon = std::vector<edge>;
using CellConnectivityGraph = std::map<edge,edge,edgeComp>;
}

namespace geometry {

class Diagram{
public:
    Diagram(){}
    const topology::SimplicialPolygon& operator[](int i) const;
    topology::edge commonEdge(int i,int j) const;
    SimplicialPolygon getCell(int i) const;

    GeometricContext G;
    std::vector<topology::SimplicialPolygon> cells;
    topology::CellConnectivityGraph CCG;
    std::set<topology::edge> edges;
    friend Diagram Voronoi(const GeometricContext& C);

    friend class cnc::DiagramPlotter;
    void build_lower_hull_dual(const GeometricContext& G,range R);
};

Diagram Voronoi(const cloud& C,range R);
Diagram Laguerre(const cloud& C,const vec& psi,range R);

}
}
}

#endif // DIAGRAMS_H
