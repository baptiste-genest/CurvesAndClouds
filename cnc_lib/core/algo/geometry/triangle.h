#ifndef TRIANGLE_H
#define TRIANGLE_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "core/algo/algo.h"
#include "core/algo/lin_alg.h"
#include "core/algo/matrix_functions.h"
#include "cnc_error.h"
#include "cnc_types.h"

namespace cnc {

namespace algo {

namespace geometry {

class Triangle
{
public:
    Triangle();
    Triangle(vec* a,vec* b,vec* c);
    Triangle(std::vector<vec>& p);

    std::array<scalar,3> get_barycentric(const vec& x) const;

    inline scalar area() const {return m_area;}
    inline scalar perimeter() const {return m_perimeter;}

    std::array<scalar,3> get_dist_to_vertices(const vec& x) const ;

    void print_caracteristics() const;

    std::vector<vec> get_edges() const;
    vec get_mid_point() const;

private:
    friend class Mesh;
    std::array<cnc::vec*,3> m_vertex;

    std::array<scalar,3> m_side_length;
    std::array<vec,3> m_sides;
    std::array<scalar,3> m_barycentric_coefficents;
    scalar m_area;
    scalar m_perimeter;

    void compute_perimeter();
    void compute_area();
    void compute_side_length();
    void compute_sides();

    inline void update_caracteristics();

    //void move_vertex(uint i,const vec& p);

    inline const vec* operator()(uint i) const { return m_vertex[i]; }
    friend bool share_vertex(const Triangle& A,const Triangle& B);
    friend bool share_edge(const Triangle& A,const Triangle& B);

    constexpr static uint dim = 2;

};

}}}

#endif // TRIANGLE_H
