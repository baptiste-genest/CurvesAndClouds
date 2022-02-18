#ifndef DIAGRAMS_H
#define DIAGRAMS_H
#include "../combinatorial_complexes.h"
#include "../geometry.h"
#include "../meshgeneration.h"
#include "../convexpolygon.h"
#include "../convex_hull.h"
#include "../../../time_handling.h"
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


namespace cnc {

class DiagramPlotter;

namespace algo {
namespace topology {
using edgeBorder = int;
using CellConnectivityGraph = std::map<edge,edge,edgeComp>;
}

namespace geometry {

class Diagram;

using DiagramRef = std::shared_ptr<Diagram>;

class Diagram{
public:
    Diagram();
    const ConvexPolygon& operator[](int i) const;
    topology::edge commonEdge(int i,int j) const;
    const ConvexPolygon& getCell(int i) const;
    GeometricContextRef getContext() const;
    ConvexPolygon polygonEdge(const topology::edge& e) const;
private:
    GeometricContextRef context;
    std::vector<ConvexPolygon> cells;
    topology::CellConnectivityGraph CCG;
    std::set<topology::edge> edges;
    friend Diagram Voronoi(const GeometricContext& C);

    friend class cnc::DiagramPlotter;
    void buildDelaunayDual(const Mesh2& G,const cloud& C,scalar R);
    scalar RV;
    friend DiagramRef Voronoi(const cloud& C,scalar R);
    friend DiagramRef Laguerre(const cloud& C,const vec& psi,scalar R);
};


DiagramRef Voronoi(const cloud& C,scalar R);
DiagramRef Laguerre(const cloud& C,const vec& psi,scalar R);

}
}
}

#endif // DIAGRAMS_H
