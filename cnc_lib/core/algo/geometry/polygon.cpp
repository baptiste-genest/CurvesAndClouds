#include "polygon.h"

cnc::algo::geometry::Polygon::Polygon(GeometricContextRef C,const std::vector<vertex>& v) : G(C),V(v)
{

}

cnc::algo::geometry::Polygon::Polygon(cnc::algo::geometry::GeometricContextRef C) : G(C)
{

}

cnc::algo::topology::vertex cnc::algo::geometry::Polygon::getNthVertex(uint n) const
{
    if (n > V.size())
        throw Cnc_error("Vertex index out of range");
    return V[n];
}

cnc::vec cnc::algo::geometry::Polygon::getNthVertexPosition(uint n) const
{
    return (*G)(getNthVertex(n));
}

cnc::scalar cnc::algo::geometry::Polygon::Area() const
{
    return std::abs(SignedArea());
}

int cnc::algo::geometry::Polygon::nbVertices() const{
    return V.size();
}

cnc::algo::geometry::indexedVertices cnc::algo::geometry::Polygon::Vertices() const
{
    return V;
}

cnc::algo::geometry::indexedVerticesCyclic cnc::algo::geometry::Polygon::getIndexedCyclicVertices() const
{
    indexedVerticesCyclic R(V.begin(),V.end());
    R.push_back(*V.begin());
    return R;
}

cnc::algo::geometry::ShapePredicate cnc::algo::geometry::Polygon::getShapePredicate() const
{
    auto R = *this;
    return [R] (const vec& x){
        return R.isInside(x);
    };
}

bool cnc::algo::geometry::Polygon::isInside(const cnc::vec &x) const
{
    auto P = getIndexedCyclicPoints();
    const auto static dir = vec({1.123,3.243432}).normalize();
    uint count = 0;
    for (int i = 0;i<nbVertices();i++){
        bool tmp;
        geometry::raySegmentIntersection(x,dir,P[i],P[i+1],tmp);
        if (tmp)
            count++;
    }
    return count%2;
}

cnc::algo::geometry::Polygon cnc::algo::geometry::Polygon::subdivide(int N) const
{
    GeometricContextRef C = std::make_shared<GeometricContext>();
    auto P = getIndexedCyclicPoints();
    uint n = P.size();
    std::vector<topology::vertex> NV;
    for (uint i = 0;i<n-1;i++){
        for (int k = 0;k<N;k++){
            scalar t = scalar(k)/N;
            NV.push_back(C.get()->add_vertex(lerp(P[i],P[i+1],t)));
        }
    }
    Polygon S(C,NV);
    return S;
}

cnc::algo::geometry::Polygon cnc::algo::geometry::Polygon::subdivideImposeLength(cnc::scalar l) const
{
    GeometricContextRef C = std::make_shared<GeometricContext>();
    auto P = getIndexedCyclicPoints();
    uint n = P.size();
    std::vector<topology::vertex> NV;
    for (uint i = 0;i<n-1;i++){
        int N = P[i].distance(P[i+1])/l;
        for (int k = 0;k<N;k++){
            scalar t = scalar(k)/N;
            NV.push_back(C.get()->add_vertex(lerp(P[i],P[i+1],t)));
        }
    }
    Polygon S(C,NV);
    return S;
}

cnc::scalar cnc::algo::geometry::Polygon::distanceTo(const cnc::vec &x) const
{
    auto V = getIndexedCyclicPoints();
    scalar mind = 1e10;
    for (uint i = 0;i<V.size()-1;i++)
        mind = std::min(SDF::toSegment(x,V[i],V[i+1]),mind);
    return mind;
}

cnc::scalar cnc::algo::geometry::Polygon::SignedArea() const
{
    if (nbVertices() < 3)
        return 0;
    scalar A = 0;
    auto V = getIndexedCyclicPoints();
    for (int i = 0;i<(int)V.size()-1;i++){
        A+= det22(V[i+1],V[i]);
    }
    return A*0.5;
}

cnc::vec cnc::algo::geometry::Polygon::Centroid() const
{
    scalar A = 0;
    vec C(2);
    auto V = getIndexedCyclicPoints();
    for (int i = 0;i<(int)V.size()-1;i++){
        scalar D = det22(V[i+1],V[i]);
        A += D;
        C += (V[i] + V[i+1])*D;
    }
    return C*(1.0/(3*A));

}

cnc::scalar cnc::algo::geometry::Polygon::Perimeter() const
{
    scalar P = 0;
    for (const auto& s: getIndexedCyclicSegments())
        P += s.norm();
    return P;
}

cnc::algo::geometry::indexedPointsCyclic cnc::algo::geometry::Polygon::getIndexedCyclicPoints() const
{
    auto P = getIndexedPoints();
    P.push_back(P[0]);
    return P;
}

cnc::algo::geometry::indexedPointsCyclic cnc::algo::geometry::Polygon::getIndexedPoints() const
{
    indexedPoints P(nbVertices());
    auto verts = Vertices();
    for (int i = 0;i<nbVertices();i++){
        P[i] = (*G)(verts[i]);
    }
    return P;
}

cnc::cloud cnc::algo::geometry::Polygon::getPointCloud() const
{
    return cloud(getIndexedPoints());
}

cnc::algo::geometry::indexedSegmentsCyclic cnc::algo::geometry::Polygon::getIndexedCyclicSegments() const
{
    auto pos = getIndexedCyclicPoints();
    indexedSegmentsCyclic S(nbVertices());
    for (int i = 0;i<nbVertices();i++)
        S[i] = pos[i+1] - pos[i];
    return S;
}

cnc::algo::geometry::indexedEdgesCyclic cnc::algo::geometry::Polygon::getIndexedCyclicEdges() const
{
    indexedEdgesCyclic E(nbVertices());
    auto v = getIndexedCyclicVertices();
    for (int i = 0;i<nbVertices();i++)
        E[i] = {v[i],v[i+1]};
    return E;
}

