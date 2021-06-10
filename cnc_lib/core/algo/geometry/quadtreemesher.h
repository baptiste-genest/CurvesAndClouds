#ifndef QUADTREEMESHER_H
#define QUADTREEMESHER_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "quadtree.h"
#include "triangle.h"
#include "geometry.h"
#include "mesh.h"
#include "../statistics/random_var.h"
#include "../bit_operations.h"

namespace cnc {

namespace algo {

namespace geometry {


class QuadtreeMesher 	{
public:
    QuadtreeMesher();
    ~QuadtreeMesher();
    void add_to_chain(const vec& x);
    void complete_chain();
    std::vector<std::vector<vec>> get_edges() const;

    Quadtree* get_quadtree(){
        return Q;
    }

    void build_mesh();
    Mesh get_mesh() const;

    bool is_triangle_in_shape(const Triangle& T);

private:

    vec* simplex_head;
    vec* simplex_tail;
    Quadtree* Q;
    std::vector<vec*> mesh_points;
    std::vector<Triangle> mesh;
    std::array<std::vector<Quadtree*>,3> leafs_by_type;
    std::vector<vec*> avg;
    std::vector<std::pair<const Quadtree*,std::pair<vec*,vec*>>> edge_intersections;
    range x_range;
    range y_range;
    uint size;
    std::vector<vec*> boundaries;
    std::vector<edge> boundaries_edge;

    void purge_outside_mesh();
    void compute_interior_mesh(Quadtree* l);
    void compute_corner_mesh(Quadtree* l);
    void compute_border_mesh(Quadtree* l);
    void subdivide_for_edges();
    std::vector<Quadtree*> get_empty_leafs_with_multiple_edges() const;
    void balance_quadtree();
    void split_leafs_by_type();
    void balance_quadtree(const std::vector<Quadtree*>& to_check);
    bool is_edge_in_box(const Quadtree* Q,const edge& e) const;
    std::array<edge,2> get_edges_by_corner(const vec* x) const;
    vec compute_quad_edge_intersection(const Quadtree* q,const edge& e) const;
    std::array<vec,2> compute_quad_edge_intersection_border(const Quadtree* q,const edge& e) const;
    std::vector<vec*> get_contact_points(Quadtree* l,uint& config);
    std::array<std::vector<vec*>,2> get_chunks(const std::vector<vec*>& points,const std::pair<vec*,vec*>& intersections);
    vec* get_avg(const std::vector<vec*>& P);
    void add_to_mesh(const vec& x);
    void compute_mesh_points();
    void make_interior_triangles(const std::vector<vec*>& points,uint config);
    vec* get_point_adress(const vec& x);
    std::vector<vec*> sort_counterclockwise(const vec& center,const std::vector<vec*>& points) const;
    scalar ray_edge_intersection(const vec& a,const vec& b,const edge& e)const;

    constexpr static scalar eps = 1e-15;
    constexpr static uint FULL_LEAF = 0;
    constexpr static uint BORDER_LEAF = 1;
    constexpr static uint VOID_LEAF = 2;
};


}}}

#endif // QUADTREEMESHER_H
