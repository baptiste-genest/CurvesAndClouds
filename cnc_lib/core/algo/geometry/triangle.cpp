#include "triangle.h"

cnc::algo::geometry::Triangle::Triangle()
{
    m_perimeter = 0;
    m_area = 0;
    for (uint k = 0;k<3;k++)
        m_side_length[k] = 0;
}

cnc::algo::geometry::Triangle::Triangle(cnc::vec* a,cnc::vec* b,cnc::vec* c) : Triangle()
{
    m_vertex[0] = a;
    m_vertex[1] = b;
    m_vertex[2] = c;
    update_caracteristics();
}

cnc::algo::geometry::Triangle::Triangle(std::vector<cnc::vec> &p) : Triangle()
{
    if (p.size() != 3)
        throw Cnc_error("A triangle must have 3 vertices not " + std::to_string(p.size()));
    for (uint k = 0;k<3;k++)
        m_vertex[k] = &p[k];
    update_caracteristics();
}

std::array<cnc::scalar, 3> cnc::algo::geometry::Triangle::get_barycentric(const vec& x) const
{
    std::array<scalar,3> B;
    vec a1 = *m_vertex[2] - x,b1 = *m_vertex[1] - x;
    vec b2 = *m_vertex[0] - x;
    B[0] = 0.5*std::abs(det22(a1,b1))/area();
    B[1] = 0.5*std::abs(det22(a1,b2))/area();
    B[2] = 1 - B[1] - B[0];
    return B;
}

std::array<cnc::scalar,3> cnc::algo::geometry::Triangle::get_dist_to_vertices(const cnc::vec &x) const
{
    std::array<scalar,3> D;
    for (uint k = 0;k<3;k++)
        D[k] = x.distance(*m_vertex[k]);
    return D;
}

void cnc::algo::geometry::Triangle::print_caracteristics() const
{
    std::cout << "area : " << area() << std::endl;
    std::cout << "perimeter : " << perimeter() << std::endl;
    std::cout << "side lengths : ";
    for (scalar s : m_side_length)
        std::cout << s << ' ';
    std::cout << std::endl;
}

std::vector<cnc::vec> cnc::algo::geometry::Triangle::get_edges() const
{
    return {*m_vertex[0],*m_vertex[1],*m_vertex[2],*m_vertex[0]};
}

cnc::vec cnc::algo::geometry::Triangle::get_mid_point() const
{
    constexpr static scalar f = scalar(1)/3;
    return (*m_vertex[0]+*m_vertex[1]+*m_vertex[2])*f;
}

void cnc::algo::geometry::Triangle::compute_side_length()
{
    for (uint k = 0;k<3;k++)
        m_side_length[k] = m_sides[k].norm();
}

void cnc::algo::geometry::Triangle::compute_sides()
{
    m_sides[0] = *m_vertex[1] - *m_vertex[0];
    m_sides[1] = *m_vertex[2] - *m_vertex[0];
    m_sides[2] = *m_vertex[2] - *m_vertex[1];
}

void cnc::algo::geometry::Triangle::compute_perimeter()
{
    m_perimeter = 0;
    for (scalar s : m_side_length)
        m_perimeter += s;
}

void cnc::algo::geometry::Triangle::compute_area()
{
    m_area = det22(m_sides[0],m_sides[1])*0.5;
    if (m_area < 0){
        m_area *= -1;
        const vec& tmp = m_sides[0];
        m_sides[0] = m_sides[1];
        m_sides[1] = tmp;
    }
}


void cnc::algo::geometry::Triangle::update_caracteristics()
{
    compute_sides();
    compute_side_length();
    compute_perimeter();
    compute_area();
}


/*
void cnc::algo::geometry::Triangle::move_vertex(uint i, const cnc::vec &p)
{
    if (p.rowNum() != dim)
        throw Cnc_error("location of a vertex must be in " + std::to_string(dim) + " dimension not " + std::to_string(p.rowNum()));
    m_vertex[i] = p;
    update_caracteristics();
}
*/

bool cnc::algo::geometry::share_vertex(const cnc::algo::geometry::Triangle &A, const cnc::algo::geometry::Triangle &B)
{
    for (uint j = 0;j<3;j++)
        for (uint i = 0;i<3;i++)
            if (A.m_vertex[i] == B.m_vertex[j])
                return true;
    return false;
}

bool cnc::algo::geometry::share_edge(const cnc::algo::geometry::Triangle &A, const cnc::algo::geometry::Triangle &B)
{
    //TODO
    return false;
}
