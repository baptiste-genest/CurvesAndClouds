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

cnc::algo::geometry::GeometricContext::GeometricContext(cnc::cloud &&C) : points(std::move(C))
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
cnc::vec& cnc::algo::geometry::GeometricContext::operator()(const cnc::algo::topology::vertex &v)
{
    return points[v];
}


cnc::vec cnc::algo::geometry::GeometricContext::midPoint(const cnc::algo::topology::face &F) const
{
    vec M(2);
    for (auto V : topology::get_vertices(F))
        M += points[V];
    return M * 0.333333;
}

cnc::vec cnc::algo::geometry::GeometricContext::midPoint(const cnc::algo::topology::edge &E) const
{
    return (points[E[0]] + points[E[1]])*0.5;
}

cnc::vec cnc::algo::geometry::GeometricContext::midPoint(const topology::vertices &V) const
{
    vec M(2);
    for (auto v : V)
        M += points[v];
    return M*(1./V.size());
}

const cnc::vec &cnc::algo::geometry::GeometricContext::getPoint(cnc::algo::topology::vertex x) const
{
    return points[x];
}

const cnc::cloud &cnc::algo::geometry::GeometricContext::getPoints() const
{
    return points;
}

cnc::cloud &cnc::algo::geometry::GeometricContext::getPoints()
{
    return points;
}

cnc::scalar cnc::algo::geometry::GeometricContext::PolygonArea(const cnc::algo::topology::SimplicialPolygon &P) const
{
    return std::abs(PolygonSignedArea(P));
}

cnc::scalar cnc::algo::geometry::GeometricContext::faceArea(const cnc::algo::topology::face &f) const
{
    auto V = topology::get_indexed_vertices(f);
    return std::abs(det22(points[V[1]]-points[V[0]],points[V[2]]-points[V[0]]))*0.5;
}

cnc::scalar cnc::algo::geometry::GeometricContext::ConvexPolygonArea(const cnc::algo::topology::SimplicialPolygon &P) const
{
    if (P.size() == 0)
        return 0;
    scalar A = 0;
    auto p = points[P[0][0]];
    for (const auto& e : P)
        A += std::abs(det22(points[e[0]]-p,points[e[1]]-p));
    return A*0.5;
}

cnc::scalar cnc::algo::geometry::GeometricContext::PolygonSignedArea(const cnc::algo::topology::SimplicialPolygon &P) const
{
    scalar A = 0;
    for (const auto& e : P)
        A += det22(points[e[0]],points[e[1]]);
    return A*0.5;
}

cnc::vec cnc::algo::geometry::GeometricContext::compute2DPolygonCentroid(const cnc::algo::topology::SimplicialPolygon &P) const
{
    vec C(2);
    scalar A = PolygonSignedArea(P);
    for (const auto& e : P){
        const auto& xc = points[e[0]];
        const auto& xn = points[e[1]];
        C(0) += (xc(0) + xn(0))*(xc(0)*xn(1) - xn(0)*xc(1));
        C(1) += (xc(1) + xn(1))*(xc(0)*xn(1) - xn(0)*xc(1));
    }
    return C/(6*A);
}

void cnc::algo::geometry::GeometricContext::uniformizeOrientation(cnc::algo::topology::SimplicialPolygon &P) const
{
    std::map<topology::vertex,topology::vertex> Adj;
    for (const auto& e : P){
        Adj[e[0]] = e[1];
        Adj[e[1]] = e[0];
    }
}

bool cnc::algo::geometry::GeometricContext::insideFace(const cnc::algo::topology::face &f, const cnc::vec &x) const
{
    static const auto Area = [] (const vec& x,const vec& y){
        return std::abs(det22(x,y)*0.5);
    };
    scalar s = 0;
    for (const auto& e: f){
        auto ve = get_vec_edge(e,e[0]);
        s += Area(x-points[e[0]],ve);
    }
    scalar A = faceArea(f);
    return s < A+1e-4;
}

cnc::scalar cnc::algo::geometry::GeometricContext::circumRadius(const cnc::algo::topology::face &F) const
{
    vec a = get_vec_edge(F[0]);
    vec b = get_vec_edge(F[1]);
    vec c = get_vec_edge(F[2]);
    scalar S = std::abs(det22(c-a,b-a))*0.5;
    return a.norm()*b.norm()*c.norm()/(4*S);
}

cnc::vec cnc::algo::geometry::GeometricContext::circumCenter(const cnc::algo::topology::face &F) const
{
    auto Vi = topology::get_indexed_vertices(F);
    vec one(3),X(3),Y(3),norms(3);
    for (uint i =0;i<3;i++){
        const auto& x = points[Vi[i]];
        one(i) = 1;
        X(i) = x(0);
        Y(i) = x(1);
        norms(i) = x.norm2();
    }
    scalar D = algo::det33(X,Y,one)*2;
    scalar Cx = algo::det33(norms,Y,one)/D;
    scalar Cy = -algo::det33(norms,X,one)/D;
    return vec({Cx,Cy});
}

cnc::algo::geometry::circum_data cnc::algo::geometry::GeometricContext::circumCenterRadius(const cnc::algo::topology::face &F) const
{
    return {circumCenter(F),circumRadius(F)};
}

cnc::scalar cnc::algo::geometry::GeometricContext::facePerimeter(const cnc::algo::topology::face &F) const
{
    scalar P = 0;
    for (const auto& e : F)
        P += get_vec_edge(e).norm();
    return P;
}

cnc::scalar cnc::algo::geometry::GeometricContext::inscribedRadius(const cnc::algo::topology::face &F) const
{
    return faceArea(F)/facePerimeter(F)*2;
}



cnc::vec cnc::algo::geometry::get_plane_dual(const vec &N)
{
    scalar f = -0.5/N(2);
    return vec({N(0)*f,N(1)*f});
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
    auto e = B-A;
    auto Me = (A+B)*0.5;
    auto eT = vec({-e(1),e(0)}).normalize();
    if (C.distance2(Me + eT) < C.distance2(Me - eT))
        eT *= -1;
    return eT;

}

cnc::scalar cnc::algo::geometry::det22(const cnc::vec &a, const cnc::vec &b)
{
    return a(0)*b(1)-a(1)*b(0);
}

cnc::vec cnc::algo::geometry::rayRayIntersection(const cnc::vec &O1, const cnc::vec &D1, const cnc::vec &O2, const cnc::vec &D2,bool& intersects)
{
    vec T;
    try {
        T = argLineLineIntersection(O1,D1,O2,D2);
    }  catch (Cnc_error&) {
        intersects = false;
        return vec(2);
    }
    intersects = (T(0) > 0 && T(0) < 1 && T(1) > 0 && T(1) < 1);
    return O1 + D1*T(0);
}

cnc::vec cnc::algo::geometry::lineLineIntersection(const cnc::vec &O1, const cnc::vec &D1, const cnc::vec &O2, const cnc::vec &D2)
{
    bool tmp;
    return rayRayIntersection(O1,D1,O2,D2,tmp);
}

cnc::vec cnc::algo::geometry::argLineLineIntersection(const cnc::vec &O1, const cnc::vec &D1, const cnc::vec &O2, const cnc::vec &D2)
{
    return solve22(mat22(D1,-D2),O2-O1);
}

cnc::vec cnc::algo::geometry::segmentSegmentIntersection(const cnc::vec &A1, const cnc::vec &A2, const cnc::vec &B1, const cnc::vec &B2, bool &intersects)
{
    return rayRayIntersection(A1,A2-A1,B1,B2-B1,intersects);
}

cnc::vec cnc::algo::geometry::raySegmentIntersection(const cnc::vec &O, const cnc::vec &D, const cnc::vec &A, const cnc::vec &B,bool& intersects)
{
    vec t;
    try {
        t = argLineLineIntersection(O,D,A,B-A);
    }  catch (Cnc_error&) {
        intersects = false;
        return vec(2);
    }
    intersects = (t(1) > 0 && t(1) < 1);
    return O + D*t(0);
}
