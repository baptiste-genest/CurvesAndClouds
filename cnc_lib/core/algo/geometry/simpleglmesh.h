#ifndef SIMPLEGLMESH_H
#define SIMPLEGLMESH_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
#include <QVector3D>
#include <cnc_error.h>
#include <QOpenGLFunctions>
#include <iostream>
#include <fstream>

#include "halfedge.h"
#include "core/algo/algo.h"
#include "core/sparse_matrix.h"

namespace cnc {


namespace algo {

namespace geometry {
class SimpleGLMesh;

struct VertexInfo {
    QVector3D vpos;
    QVector3D normal;
    QVector3D color;
    scalar value;
    std::vector<halfedge*> emerging;
    uint id;
};

using vref = uint;
using href = halfedge*;
using qvr = const QVector3D&;

class SimpleGLMesh
{
public:
    SimpleGLMesh() {}
    SimpleGLMesh(const std::vector<QVector3D>&,const std::vector<QVector3D>&,const std::vector<uint>&);
    SimpleGLMesh(int nb_vertex,int nb_face);
    void set_vertex_pos(uint vertex_id,const QVector3D& v);
    void set_vertex_normal(uint vertex_id,const QVector3D& v);
    void set_vertex_color(uint vertex_id,const QVector3D& v);
    void set_vertex_value(uint vertex_id,scalar v);
    void set_vertex_values(const vec& v);
    void set_face(uint face_id,uint v1,uint v2,uint v3);

    VertexInfo* get_vertex(uint vertex_id){
        if (vertex_id >= nb_vertices)
            throw Cnc_error("vertex id out of range");
        return &vertices[vertex_id];
    }

    void compute_normals();

    inline std::vector<VertexInfo> get_points() const{
        return vertices;
    }

    inline std::vector<uint> get_faces() const {
        return faces;
    }
    inline std::vector<uint> get_face(uint f) const {
        return {faces[f*3],faces[f*3+1],faces[f*3+2]};
    }
    inline uint get_nb_vertices() const {
        return nb_vertices;
    }

    inline uint get_nb_faces() const {
        return nb_faces;
    }

    inline QVector3D get_vpos(uint k) const {
        return vertices[k].vpos;
    }

    inline QVector3D get_vpos_by_face(uint f,uint i) const {
        return vertices[faces[f*3+i]].vpos;
    }

    inline QVector3D get_mean_point() const {
        QVector3D mean;
        for (const auto& p : vertices)
            mean += p.vpos;
        return mean/nb_vertices;
    }

    inline vec get_vertex_values() const;

    template <class T>
    T laplace_beltrami_operator(vref x,const std::function<T(const VertexInfo*)>& fpv) const{
        std::vector<vref> N = one_ring_neighbors(x);
        if (N.size() == 0)
            throw Cnc_error("vertex has no neighbors");
        T LB = T();
        T val_at_vertex = fpv(&vertices[x]);
        for (vref n : N)
            LB += cotan_weight(x,n)*(fpv(&vertices[n])-val_at_vertex);
        return LB/(1e-6 + barycentric_area(x));
    }

    std::vector<uint> get_boundary_vertices() const;


    mat compute_laplace_beltrami_matrix(bool weighted = true) const;
    mat compute_uniform_laplacian_matrix() const ;
    smat compute_sparse_laplace_beltrami_matrix(bool weighted = true,bool pos = true) const;
    smat compute_identity_plus_dt_sparse_laplace_beltrami_matrix(scalar dt = scalar(1),bool weighted = true,bool pos = true) const;
    smat compute_weight_plus_dt_cot_matrix(scalar dt = scalar(1)) const;
    smat compute_mass_matrix() const;
    scalar mean_curvature(vref x) const;

    std::vector<QVector3D> gradient_per_triangle() const;
    std::vector<QVector3D> gradient_per_triangle(const vec& v) const;
    vec integrated_divergence_per_vertex(const std::vector<QVector3D>& v,bool pos_cotan = true) const;

    scalar mean_spacing() const;

    vec get_geodesic_distance_from_vertex(uint vertex_id) const;

    std::vector<uint> get_minimizer_simplex(uint vertex_start,const vec& f) const;

    ~SimpleGLMesh();

    vec test();

    void initialize_halfedges(bool opposites = true);
    scalar face_area(uint f) const ;

    void set_iso_lines_color(uint nb_lines);
    void truncate_mesh(const std::function<bool(qvr)>& exclude_condition);

private:

    QVector3D get_face_normal(uint f) const;

    std::vector<uint> get_faces(uint vid) const;
    void set_halfedge(int HE_id,int face_id);
    void set_opposites();

    halfedge* get_edge(vref a,vref b,uint face) const;
    halfedge* get_edge(vref a,vref b) const;
    bool are_well_oriented(halfedge* h1,halfedge* h2) const;
    scalar barycentric_area(vref v) const ;
    scalar sum_faces_areas(vref x) const ;
    scalar area_from_halfedge(halfedge* h) const ;
    scalar cotan_weight(vref a,vref b,bool pos = true) const;
    scalar cotan_weight(halfedge* h,bool pos = true) const;
    std::vector<vref> one_ring_neighbors(vref x) const;
    bool share_edge(uint f1,uint f2,uint*) const;

    std::vector<VertexInfo> vertices;
    std::vector<uint> faces;
    std::vector<halfedge*> halfedges;


    uint nb_vertices;
    uint nb_faces;
};

}
}
}

#endif // SIMPLEGLMESH_H
