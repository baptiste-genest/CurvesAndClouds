#ifndef CONVEXPOLYGON_H
#define CONVEXPOLYGON_H

#include "geometry.h"
#include "polygon.h"
#include <list>

namespace cnc {

namespace algo {

namespace geometry {
using convexVertices = std::list<topology::vertex>;

class ConvexPolygon : public Polygon
{
public:
    ConvexPolygon(GeometricContextRef C);
    ConvexPolygon();
    void insert(topology::vertex x);
    void insert(const vec& x);

    scalar segLength() const;

    const convexVertices& vertices() const;

    ConvexPolygon subdivide(int N) const;

    int nbVertices() const override;
    ShapePredicate getShapePredicate() const override;
    vertex getNthVertex(uint n) const override;
    virtual indexedVertices Vertices() const override;
    indexedVerticesCyclic getIndexedCyclicVertices() const override;
    bool isInside(const vec& x) const override;

private:
    convexVertices V;

    static bool pointsToward(const vec& A1,const vec& A2,const vec& B1,const vec& B2);
    static bool inHalfPlane(const vec& x,const vec& pm,const vec& pp);
    friend ConvexPolygon ConvexPolygonIntersection(const ConvexPolygon& P1,const ConvexPolygon& P2);
};

namespace convexPrimitive {
ConvexPolygon Square(scalar sidelength,vec center = vec(2));
ConvexPolygon Rectangle(scalar width,scalar height,vec center = vec(2));
ConvexPolygon Circle(scalar radius, int rad,vec center = vec(2));
ConvexPolygon Segment(const vec& a,const vec& b);
ConvexPolygon Triangle(const vec& a,const vec& b,const vec& c);
}

ShapePredicate Union(const ShapePredicate& A,const ShapePredicate& B);
ShapePredicate Inter(const ShapePredicate& A,const ShapePredicate& B);
ShapePredicate Not(const ShapePredicate& A);

ConvexPolygon ConvexPolygonIntersection(const ConvexPolygon& P1,const ConvexPolygon& P2);

}

}

}


#endif // CONVEXPOLYGON_H
