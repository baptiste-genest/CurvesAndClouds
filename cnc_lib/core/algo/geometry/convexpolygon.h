#ifndef CONVEXPOLYGON_H
#define CONVEXPOLYGON_H

#include "geometry.h"
#include <list>

namespace cnc {

namespace algo {

namespace geometry {

using convexVertices = std::list<topology::vertex>;
using indexedConvexVerticesCyclic = std::vector<topology::vertex>;
using indexedConvexPointsCyclic = std::vector<vec>;
using indexedConvexSegmentsCyclic = std::vector<vec>;

class ConvexPolygon
{
public:
    ConvexPolygon(GeometricContextRef C);
    ConvexPolygon();
    void insert(topology::vertex x);
    void insert(const vec& x);
    const convexVertices& vertices() const;
    indexedConvexVerticesCyclic getIndexedCyclicVertices() const;
    indexedConvexPointsCyclic getIndexedCyclicPoints() const;
    indexedConvexSegmentsCyclic getIndexedCyclicSegments() const;
    scalar Area() const;
    scalar Perimeter() const;
    scalar segLength() const;
    bool isInside(const vec& x) const;

private:
    convexVertices V;
    GeometricContextRef G;

    static bool pointsToward(const vec& A1,const vec& A2,const vec& B1,const vec& B2);
    static bool inHalfPlane(const vec& x,const vec& pm,const vec& pp);
    friend ConvexPolygon ConvexPolygonIntersection(const ConvexPolygon& P1,const ConvexPolygon& P2);
};

namespace convexPrimitive {
ConvexPolygon Square(scalar sidelength,vec center = vec(2));
ConvexPolygon Rectangle(scalar width,scalar height,vec center = vec(2));
ConvexPolygon Circle(scalar radius, int rad,vec center = vec(2));
ConvexPolygon Segment(const vec& a,const vec& b);
}

ConvexPolygon ConvexPolygonIntersection(const ConvexPolygon& P1,const ConvexPolygon& P2);

}

}

}


#endif // CONVEXPOLYGON_H
