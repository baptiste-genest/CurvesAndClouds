#ifndef COMBINATORIAL_COMPLEXES_H
#define COMBINATORIAL_COMPLEXES_H
#include "cnc_types.h"
#include <set>
#include <vector>
#include <array>
#include "../lin_alg.h"
#include "../matrix_functions.h"
#include "../statistics/statistics.h"

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

namespace cnc{

namespace algo {

namespace topology {

using vertex = int;
using vertices = std::set<vertex>;
using indexed_vertices = std::vector<vertex>;
using edge = std::array<vertex,2>;
using face = std::array<edge,3>;
using SimplicialPolygon = std::vector<edge>;

struct faceComp{//true if set(vertices) are equal
    bool operator()(const face& a,const face& b);
    bool operator()(const face& a,const face& b)const;
};
struct edgeComp{
    bool operator()(const edge& a,const edge& b);
    bool operator()(const edge& a,const edge& b)const;
};

using faces = std::set<face,faceComp>;
using edges = std::set<edge,edgeComp>;
using indexed_edges = std::vector<edge>;
using EdgeFaceConnectivityGraph = std::map<edge,faces,edgeComp>;

bool operator==(const edge& a,const edge& b);
bool operator==(const face& a,const face& b);

bool belong(vertex x,const face& F);
bool belong(vertex x,const edge& E);
bool belong(const edge& E,const face& F);

vertices get_vertices(const face& F);
vertices get_vertices(const SimplicialPolygon& P);
indexed_vertices get_indexed_vertices(const face& F);
indexed_vertices get_indexed_vertices(const SimplicialPolygon& P);
edges get_edges(const face& F);
edge get_common_edge(const face& F,vertex O);
vertex get_other(const face& F,const edge& e);
vertex get_other(const edge& F,vertex x);
face assemble_face(const edge& E,const vertex& x);
void swap(edge& E);

edge completeCycle(const SimplicialPolygon& P);

constexpr static edge NULL_EDGE{-1,-1};

}
}
}

#endif // COMBINATORIAL_COMPLEXES_H
