#ifndef MESH2_H
#define MESH2_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
#include "geometry.h"
#include "combinatorial_complexes.h"

namespace cnc {

namespace algo {

namespace geometry {

using namespace topology;

using NormalMap = std::map<topology::face,vec,topology::faceComp>;

class Mesh2;

using trianglePredicate = std::function<bool(const Mesh2&,const topology::face&)>;

class Mesh2
{
public:
    Mesh2(const GeometricContext& C,const topology::faces& Fa,const EdgeFaceConnectivityGraph& EF,const NormalMap& NM);

    vec getNormal(const face& F) const;
    Mesh2 getSubMesh(const trianglePredicate& P) const;

    const topology::faces& faces() const;
    const topology::faces& getFaces(const topology::edge& e) const;
    std::vector<topology::face> getIndexedFaces(const topology::edge& e) const;
    const topology::edges& edges() const;
    const topology::edges& interiorEdges() const;
    const topology::edges& boundaryEdges() const;
    const topology::edge& getBoundaryEdge(const topology::face& F) const;
    int nbVertices() const;
    topology::face get_opposite(const topology::face& F,const topology::edge& e) const;

//private:
    Mesh2(const GeometricContext& C) : G(C) {}
    void induce_from_faces();

    const GeometricContext& G;
    topology::vertices V;
    topology::edges E;
    topology::faces F;
    topology::edges boundary;
    topology::edges interior_edges;
    topology::EdgeFaceConnectivityGraph EtoF;
    NormalMap N;
};

}}}

#endif // MESH2_H
