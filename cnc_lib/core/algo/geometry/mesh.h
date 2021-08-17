#ifndef MESH_H
#define MESH_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "triangle.h"
#include "halfedge.h"

#include "core/algo/algo.h"
#include "core/algo/lin_alg.h"
#include "cnc_error.h"
#include "cnc_types.h"

namespace cnc {

namespace algo {

namespace geometry {

class Mesh
{
public:
    Mesh();
    ~Mesh();
    Mesh(const std::vector<Triangle>& T);
    inline void add_face(const Triangle& t);
    inline const std::vector<Triangle>& get_faces() {
        return faces;
    }

private:

    void insert_vertex_from_adress(vec*);
    int nb_vertex = 0;

    std::vector<halfedge*> h_edges;
    std::vector<Triangle> faces;
    std::vector<vec> vertices;
};

}}}


#endif // MESH_H
