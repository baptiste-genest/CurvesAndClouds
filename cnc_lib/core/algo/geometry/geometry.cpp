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

cnc::vec cnc::algo::geometry::GeometricContext::get_vec_edge(const cnc::algo::topology::edge &e) const {
    return points[e[1]] - points[e[0]];
}

cnc::vec cnc::algo::geometry::GeometricContext::get_vec_edge(const cnc::algo::topology::edge &e, cnc::algo::topology::vertex x) const
{
    return points[topology::get_other(e,x)] - points[x];
}

cnc::vec cnc::algo::geometry::GeometricContext::face_direction(const cnc::algo::topology::edge &E, cnc::algo::topology::vertex O) const {
    auto ev = get_vec_edge(E).normalize();
    auto ov = get_vec_edge({E[0],O});
    return algo::cross(algo::cross(ev,ov).normalize(),ev);
}

cnc::scalar cnc::algo::geometry::GeometricContext::facet_angle(const cnc::algo::topology::face &F,const cnc::algo::topology::edge& E, cnc::algo::topology::vertex O)const {
    auto CEv = get_vec_edge(E);
    return algo::dot(face_direction(E,O),face_direction(E,topology::get_other(F,E)));
}

cnc::algo::topology::vertices cnc::algo::geometry::GeometricContext::getVertices() const
{
    topology::vertices V;
    for (int i = 0;i<(int)points.size();i++)
        V.insert(i);
    return V;
}

cnc::algo::topology::indexed_vertices cnc::algo::geometry::GeometricContext::getIndexedVertices() const
{
    topology::indexed_vertices V(points.size());
    std::iota(V.begin(),V.end(),0);
    return V;
}

cnc::algo::topology::vertex cnc::algo::geometry::GeometricContext::add_vertex(const cnc::vec &x)
{
    topology::vertex V = points.size();
    points.add_point(x);
    return V;
}


cnc::vec cnc::algo::geometry::GeometricContext::operator()(const cnc::algo::topology::vertex &v) const
{
    return points[v];
}

cnc::vec cnc::algo::geometry::GeometricContext::midPoint(const cnc::algo::topology::face &F) const
{
    vec M(3);
    for (auto V : topology::get_vertices(F))
        M += points[V];
    return M * 0.333333;
}

cnc::vec cnc::algo::geometry::GeometricContext::midPoint(const cnc::algo::topology::edge &E) const
{
    return (points[E[0]] + points[E[1]])*0.5;
}

const cnc::cloud &cnc::algo::geometry::GeometricContext::getPoints() const
{
    return points;
}

cnc::cloud &cnc::algo::geometry::GeometricContext::getPoints()
{
    return points;
}

cnc::vec cnc::algo::geometry::get_plane_dual(const vec &N)
{
    scalar f = -0.5/N(2);
    return vec({N(0)*f,N(1)*f,0});
}

cnc::vec cnc::algo::geometry::get_intersection(const cnc::algo::geometry::line &A, const cnc::algo::geometry::line &B)
{
    //A = line from A(0) of dir A(1)
    //a.o + a.d*t = b.o + b.d*t
    //t = b.o-a.o/(a.d-b.d)
    for (int i = 0;i<2;i++){
        auto denum = A.second(i)-B.second(i);
        if (std::abs(denum) > 1e-8)
            return A.first + A.second*((B.first(i) - A.first(i))/denum);
    }
    throw Cnc_error("couldn't compute line line intersection");
}

cnc::vec cnc::algo::geometry::smallest_positive_ray_square_intersection(const cnc::vec &O, const cnc::vec &D, cnc::scalar R)
{
    std::vector<scalar> roots;
    for (int i = 0;i<(int)O.size();i++)
        roots.push_back(calculus::smallest_positive_quadratic_root(D(i)*D(i),2*O(i)*D(i),O(i)*O(i) - R*R));
    return O + D*(*std::min_element(roots.begin(),roots.end()));
}

cnc::vec cnc::algo::geometry::get2DOutwardNormal(const cnc::vec &A, const cnc::vec &B, const cnc::vec &C)
{
    static auto P = [] (const vec& x){
        return vec({x(0),x(1),0});
    };
    auto e = P(B-A);
    auto Me = P(A+B)*0.5;
    auto M = P(Me*2+C)*0.3333;
    auto eT = vec({-e(1),e(0),0}).normalize();
    scalar dx = 1e-3;
    if (M.distance2(Me + eT*dx) < M.distance2(Me - eT*dx))
        eT *= -1;
    return eT;

}
