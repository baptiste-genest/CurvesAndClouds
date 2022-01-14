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
class Valued2DMeshDisplayer;

namespace algo {

namespace FEM {  class FiniteElementSolver;}
namespace geometry {

using namespace topology;

using NormalMap = std::map<topology::face,vec,topology::faceComp>;

class Mesh2;

using trianglePredicate = std::function<bool(const Mesh2&,const topology::face&)>;

using MeshRef = std::shared_ptr<Mesh2>;

class Mesh2
{
public:
    Mesh2(GeometricContextRef C,const topology::faces& Fa,const EdgeFaceConnectivityGraph& EF,const NormalMap& NM);
    Mesh2(GeometricContextRef C,const topology::faces& Fa);
    Mesh2(Mesh2&& O);
    Mesh2(const Mesh2& other);

    vec getNormal(const face& F) const;
    Mesh2 getSubMesh(const trianglePredicate& P) const;
    void filterFaces(const trianglePredicate& P);

    const topology::faces& faces() const;
    const topology::vertices& getVertices() const;
    const topology::faces& getFaces(const topology::edge& e) const;
    std::vector<topology::face> getIndexedFaces(const topology::edge& e) const;
    const topology::edges& edges() const;
    const topology::edges& interiorEdges() const;
    const topology::edges& boundaryEdges() const;

    inline const topology::vertices& interiorVertices() const{
        return interior_vertices;
    }

    const topology::edge& getBoundaryEdge(const topology::face& F) const;
    int nbVertices() const;
    topology::face get_opposite(const topology::face& F,const topology::edge& e) const;
    vec getVertexPosition(vertex v) const;

    std::array<topology::face,3> insertVertexInFace(const face& F,const vec& x);
    GeometricContextRef getContext();
    const GeometricContextRef getContext() const;
    void insertFace(const topology::face& f);
    void computeConnectivity();
    bool isInteriorVertex(vertex x) const;
    bool isBoundaryEdge(const topology::edge& e) const;
    bool isBoundaryVertex(vertex v) const;
    const topology::face& getOppositeFace(const topology::face& f,const topology::edge& e) const;
    const topology::faces& getAdjacentFaces(const topology::edge& e) const;
    const topology::faces& getOneRingFaces(topology::vertex v) const;
    topology::vertices getOneRingVertices(topology::vertex v) const;
    const topology::edges &getOneRingEdges(topology::vertex v) const;

    topology::edge edgeFlip(const topology::edge& e);
    topology::vertex edgeSplit(const topology::edge& e);
    void edgeCollapse(const topology::edge& e);
    void edgeCollapseInto(const topology::edge& e,topology::vertex v);

    bool shouldFlip(const topology::edge& e) const;

    int vertexValence(topology::vertex v) const;
    int valenceScore(const topology::edge& e) const;

    vec oneRingBarycenter(topology::vertex v) const;

    scalar maxInscribedRadius() const;

    const topology::vertices& getInteriorVertices() const;

    cloud getVertexCloud() const;

    void filterDeadVertices();
    topology::edge mf;

    topology::vertices getBoundaryVertices() const;

private:
    bool compute_connectivity = false;
    Mesh2(GeometricContextRef C) : G(C) {}
    void induce_from_faces();
    void induce_face_connectivity(const topology::face& f);
    void induce_edge_connectivity(const topology::edge& e);

    void clear_connectivity();

    void checkConnectivity() const {
        if (!compute_connectivity)
            throw Cnc_error("Connectivity not computed for this mesh!");
    }
    void checkEuler() const {
        if (!compute_connectivity)
            return;
        auto euler = V.size() - E.size() + F.size();
        if (euler != 1)
            throw Cnc_error("EULER FORMULA PROBLEM");
    }
    void remove_edge(const topology::edge& e);
    void insert_edge(const topology::edge& e);
    void remove_face(const topology::face& f);
    topology::vertex add_vertex(const vec& x);
    void remove_vertex(topology::vertex v);

    GeometricContextRef G;

    topology::vertices V;
    topology::edges E;
    topology::faces F;

    topology::edges boundary;
    topology::edges interior_edges;

    topology::vertices interior_vertices;
    topology::vertices boundary_vertices;

    topology::EdgeFaceConnectivityGraph EtoF;
    topology::VertexFaceConnectivityGraph VtoF;
    topology::VertexEdgeConnectivityGraph VtoE;

    NormalMap N;

    friend class cnc::Mesh2DDisplayer;
    friend class cnc::Valued2DMeshDisplayer;
    friend struct mesh_generation;
};

}}}

#endif // MESH2_H
