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
class Mesh2DDisplayer;

namespace algo {

namespace geometry {

using namespace topology;

using NormalMap = std::map<topology::face,vec,topology::faceComp>;

class Mesh2;

using trianglePredicate = std::function<bool(const Mesh2&,const topology::face&)>;

class Mesh2
{
public:
    Mesh2(GeometricContextRef C,const topology::faces& Fa,const EdgeFaceConnectivityGraph& EF,const NormalMap& NM);
    Mesh2(Mesh2&& O);

    vec getNormal(const face& F) const;
    Mesh2 getSubMesh(const trianglePredicate& P) const;
    void filterFaces(const trianglePredicate& P);

    const topology::faces& faces() const;
    const topology::faces& getFaces(const topology::edge& e) const;
    std::vector<topology::face> getIndexedFaces(const topology::edge& e) const;
    const topology::edges& edges() const;
    const topology::edges& interiorEdges() const;
    const topology::edges& boundaryEdges() const;
    const topology::edge& getBoundaryEdge(const topology::face& F) const;
    int nbVertices() const;
    topology::face get_opposite(const topology::face& F,const topology::edge& e) const;
    vec getVertexPosition(vertex v) const;

    std::array<topology::face,3> insertVertexInFace(const face& F,const vec& x);
    GeometricContextRef getContext();
    void insertFace(const topology::face& f);

private:
    Mesh2(GeometricContextRef C) : G(C) {}
    void induce_from_faces();

    GeometricContextRef G;
    topology::vertices V;
    topology::edges E;
    topology::faces F;
    topology::edges boundary;
    topology::edges interior_edges;
    topology::EdgeFaceConnectivityGraph EtoF;
    friend class cnc::Mesh2DDisplayer;
    friend struct mesh_generation;
    NormalMap N;
};

}}}

#endif // MESH2_H
