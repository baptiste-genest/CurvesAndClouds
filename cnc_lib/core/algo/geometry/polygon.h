#ifndef POLYGON_H
#define POLYGON_H

#include "geometry.h"

namespace cnc {

namespace algo {
using namespace topology;

namespace geometry {
using ShapePredicate = std::function<bool(const vec& x)>;
using indexedVerticesCyclic = std::vector<topology::vertex>;
using indexedVertices = std::vector<topology::vertex>;
using indexedEdgesCyclic = std::vector<topology::edge>;
using indexedPointsCyclic = std::vector<vec>;
using indexedPoints = std::vector<vec>;
using indexedSegmentsCyclic = std::vector<vec>;


class Polygon
{
public:
    Polygon(GeometricContextRef C,const std::vector<vertex>& v);

    virtual vertex getNthVertex(uint n) const;
    virtual int nbVertices() const;
    virtual indexedVertices Vertices() const;
    virtual indexedVerticesCyclic getIndexedCyclicVertices() const;
    virtual ShapePredicate getShapePredicate() const;
    virtual bool isInside(const vec& x) const;

    Polygon subdivide(int N) const;
    Polygon subdivideImposeLength(scalar l) const;

    vec getNthVertexPosition(uint n) const;
    scalar Area() const;
    scalar SignedArea() const;
    vec Centroid() const;
    scalar Perimeter() const;
    cloud getPointCloud() const;

    indexedPointsCyclic getIndexedCyclicPoints() const;
    indexedPointsCyclic getIndexedPoints() const;
    indexedSegmentsCyclic getIndexedCyclicSegments() const;
    indexedEdgesCyclic getIndexedCyclicEdges() const;

protected:
    Polygon(GeometricContextRef C);
    GeometricContextRef G;
private:
    indexedVertices V;
};

}

}

}

#endif // POLYGON_H
