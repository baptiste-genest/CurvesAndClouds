#ifndef MESH_H
#define MESH_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "triangle.h"

#include "core/algo/algo.h"
#include "core/algo/lin_alg.h"
#include "cnc_error.h"
#include "cnc_types.h"

namespace cnc {

namespace algo {

namespace geometry {

class vertex_ref;

class Mesh
{
public:
    Mesh();
    inline vertex_ref add_vertex(const vec& x);
    inline void add_face(const Triangle& t);

private:
    friend class vertex_ref;
    std::vector<vec> vertex;
    std::vector<Triangle> faces;
    constexpr static uint dim = 2;
};

class vertex_ref {
public:
    vertex_ref(uint i,Mesh* c) : id(i),container(c) {}
    vec operator()() const ;
private:
    uint id;
    Mesh* container;
};

}}}


#endif // MESH_H
