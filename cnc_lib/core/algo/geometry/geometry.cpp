#include "geometry.h"

cnc::scalar cnc::algo::geometry::isotropy_score(const cnc::vec &a, const cnc::vec &b, const cnc::vec &c)
{
    std::array<vec,3> v = {a,b,c};
    return isotropy_score(v);
}

cnc::scalar cnc::algo::geometry::isotropy_score(const std::array<cnc::vec,3> &v)
{
    scalar score = 0;
    constexpr static scalar deg60 = M_PI/3;
    for (uint k = 0;k<3;k++){
        vec split = v[(k+1)%3] - v[k];
        scalar angle = std::abs(std::atan2(split(1),split(0)));
        score = std::pow(deg60-angle,2);
    }
    return score;
}

cnc::vec cnc::algo::geometry::cross(const cnc::vec &a, const cnc::vec &b)
{
    if (a.rowNum() != 3 || b.rowNum() != 3)
        throw Cnc_error("can't apply cross product to non 3 dimensional vectors");
    vec c(3);
    c(0) = a(1)*b(2) - a(2)*b(1);
    c(1) = a(2)*b(0) - a(0)*b(2);
    c(2) = a(0)*b(1) - a(1)*b(0);
    return c;
}

cnc::algo::geometry::GeometricContext::GeometricContext(const cloud &C) : points(C)
{
}

cnc::vec cnc::algo::geometry::GeometricContext::get_vec_edge(const cnc::algo::topo::edge &e){
    return points[e[1]] - points[e[0]];
}

cnc::vec cnc::algo::geometry::GeometricContext::face_direction(const cnc::algo::topo::edge &E, cnc::algo::topo::vertex O){
    auto ev = get_vec_edge(E).normalize();
    auto ov = get_vec_edge({E[0],O});
    return algo::cross(algo::cross(ev,ov).normalize(),ev);
}

cnc::scalar cnc::algo::geometry::GeometricContext::facet_angle(const cnc::algo::topo::face &F, cnc::algo::topo::vertex O){
    auto CE = topo::get_common_edge(F,O);
    auto CEv = get_vec_edge(CE);
    return algo::dot(face_direction(CE,O),face_direction(CE,topo::get_other(F,CE)));
}
