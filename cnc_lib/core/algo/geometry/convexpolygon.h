#ifndef CONVEXPOLYGON_H
#define CONVEXPOLYGON_H

#include "geometry.h"
#include <list>

namespace cnc {

namespace algo {

namespace geometry {

using convexVertices = std::list<topology::vertex>;
using indexedConvexVerticesCyclic = std::vector<topology::vertex>;
using indexedConvexEdgesCyclic = std::vector<topology::edge>;
using indexedConvexPointsCyclic = std::vector<vec>;
using indexedConvexSegmentsCyclic = std::vector<vec>;
using ShapePredicate = std::function<bool(const vec& x)>;

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
    indexedConvexPointsCyclic getIndexedPoints() const;
    cloud getPointCloud() const;
    indexedConvexSegmentsCyclic getIndexedCyclicSegments() const;
    indexedConvexEdgesCyclic getIndexedCyclicEdges() const;
    ConvexPolygon subdivide(int N) const;
    scalar Area() const;
    scalar SignedArea() const;
    vec Centroid() const;
    scalar Perimeter() const;
    scalar segLength() const;
    bool isInside(const vec& x) const;
    int nbVertices() const;
    ShapePredicate getShapePredicate() const;

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
