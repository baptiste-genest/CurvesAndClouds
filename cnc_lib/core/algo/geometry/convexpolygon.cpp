#include "convexpolygon.h"

cnc::algo::geometry::ConvexPolygon::ConvexPolygon(GeometricContextRef C) : G(C)
{

}

cnc::algo::geometry::ConvexPolygon::ConvexPolygon()
{
    G = std::make_shared<GeometricContext>();
}

void cnc::algo::geometry::ConvexPolygon::insert(cnc::algo::topology::vertex x)
{
    if (std::find(V.begin(),V.end(),x) != V.end())
        return;
    const GeometricContext& gc = *G;
    if (V.empty()){
        V.push_back(x);
        return;
    }
    if (V.size() == 1){
        const auto& op = gc(*V.begin());
        const auto& p = gc(x);
        if (op(0) < p(0))
            V.push_front(x);
        else
            V.push_back(x);
        return;
    }
    auto n = V.begin();
    ++n;
    for (auto v = V.begin();n != V.end();++v,++n){
        const auto& c = gc(*v);
        const auto& nv = gc(*n);
        if (det22(nv-c,gc(x)-c) > 0){
            V.insert(n,x);
            return;
        }
    }
    V.insert(V.begin(),x);
}

void cnc::algo::geometry::ConvexPolygon::insert(const cnc::vec &x)
{
    insert(G->add_vertex(x));
}

const cnc::algo::geometry::convexVertices &cnc::algo::geometry::ConvexPolygon::vertices() const
{
    return V;
}

cnc::algo::geometry::indexedConvexVerticesCyclic cnc::algo::geometry::ConvexPolygon::getIndexedCyclicVertices() const
{
    indexedConvexVerticesCyclic R(V.begin(),V.end());
    R.push_back(*V.begin());
    return R;
}

cnc::algo::geometry::indexedConvexPointsCyclic cnc::algo::geometry::ConvexPolygon::getIndexedCyclicPoints() const
{
    indexedConvexPointsCyclic P(V.size()+1);
    int i = 0;
    const GeometricContext& gc = *G;
    for (auto v : V){
        P[i] = gc(v);
        i++;
    }
    P.back() = gc(*V.begin());
    return P;
}

cnc::algo::geometry::indexedConvexSegmentsCyclic cnc::algo::geometry::ConvexPolygon::getIndexedCyclicSegments() const
{
    indexedConvexSegmentsCyclic S;
    const GeometricContext& gc = *G;
    auto n = V.begin();
    ++n;
    int i = 0;
    for (auto v = V.begin();n != V.end();++v,++n){
        const auto& c = gc(*v);
        const auto& nv = gc(*n);
        S[i] = nv-c;
        i++;
    }
    return S;
}

cnc::scalar cnc::algo::geometry::ConvexPolygon::Area() const
{
    if (V.size() < 3)
        return 0;
    scalar A = 0;
    auto V = getIndexedCyclicPoints();
    for (int i = 1;i<(int)V.size()-2;i++)
        A+= std::abs(det22(V[i+1]-V[0],V[i]-V[0]));
    return A*0.5;
}

cnc::scalar cnc::algo::geometry::ConvexPolygon::Perimeter() const
{
    scalar P = 0;
    for (const auto& s: getIndexedCyclicSegments())
        P += s.norm();
    return P;
}

cnc::scalar cnc::algo::geometry::ConvexPolygon::segLength() const
{
    if (V.size() < 2)
        return 0;
    auto b = V.begin();
    const auto& x1 = (*G)(*b);
    ++b;
    const auto& x2 = (*G)(*b);
    return x1.distance(x2);
}

bool cnc::algo::geometry::ConvexPolygon::isInside(const cnc::vec &x) const
{
    auto V = getIndexedCyclicPoints();
    for (int i = 0;i<(int)V.size()-1;i++){
        if (det22(V[i+1]-V[i],x-V[i]) > 0)
            return false;
    }
    return true;
}

bool cnc::algo::geometry::ConvexPolygon::pointsToward(const cnc::vec &A1, const cnc::vec &A2, const cnc::vec &B1, const cnc::vec &B2)
{
    vec T;
    try {
        T = argLineLineIntersection(A1,A2-A1,B1,B2-B1);
    }  catch (Cnc_error&) {
        return false;
    }
    return T(0) > 1;
}

bool cnc::algo::geometry::ConvexPolygon::inHalfPlane(const cnc::vec &x, const cnc::vec &pm, const cnc::vec &pp)
{
    return det22(pp-pm,x-pm) <= 0;
}

cnc::algo::geometry::ConvexPolygon cnc::algo::geometry::ConvexPolygonIntersection(const cnc::algo::geometry::ConvexPolygon &P1, const cnc::algo::geometry::ConvexPolygon &P2)
{
    ConvexPolygon I;
    if (P1.V.empty() || P2.V.empty())
        return I;
    static constexpr int noinside = 0, ainside = 1,binside = 2;
    auto V1 = P1.getIndexedCyclicPoints();
    auto V2 = P2.getIndexedCyclicPoints();
    int nA = V1.size()-1,nB = V2.size()-1;
    int a = 0,b = 0;
    int inside = noinside;
    uint max = 2*(nA+nB);
    int nb_inter = 0;
    for (uint iter = 0;iter < max;iter++){
        const vec& ca = V1[a%nA];
        const vec& na = V1[(a+1)%nA];
        const vec& cb = V2[b%nB];
        const vec& nb = V2[(b+1)%nB];
        bool inter = false;
        auto x = segmentSegmentIntersection(ca,na,cb,nb,inter);
        if (inter){
            nb_inter++;
            if (nb_inter == 3)
                return I;
            else{
                if (ConvexPolygon::inHalfPlane(na,cb,nb))
                    inside = ainside;
                else
                    inside = binside;
            }
            I.insert(x);
        }
        if (det22(nb-cb,na-ca) <= 0){
            if (ConvexPolygon::inHalfPlane(na,cb,nb)){
                if (inside == binside)
                    I.insert(nb);
                b++;
            }
            else {
                if (inside == ainside)
                    I.insert(na);
                a++;
            }
        }
        else{
            if (ConvexPolygon::inHalfPlane(nb,ca,na)){
                if (inside == ainside)
                    I.insert(na);
                a++;
            }
            else {
                if (inside == binside)
                    I.insert(nb);
                b++;
            }
        }
    }
    if (P1.isInside(V2[0]))
        return P2;
    if (P2.isInside(V1[0]))
        return P1;
    return ConvexPolygon();
}

cnc::algo::geometry::ConvexPolygon cnc::algo::geometry::convexPrimitive::Square(cnc::scalar sidelength, cnc::vec center)
{
    ConvexPolygon P;
    scalar s = sidelength*0.5;
    P.insert(center + vec({s,s}));
    P.insert(center + vec({s,-s}));
    P.insert(center + vec({-s,s}));
    P.insert(center + vec({-s,-s}));
    return P;
}

cnc::algo::geometry::ConvexPolygon cnc::algo::geometry::convexPrimitive::Rectangle(cnc::scalar width, cnc::scalar height, cnc::vec center)
{
    ConvexPolygon P;
    scalar w = width*0.5;
    scalar h = height*0.5;
    P.insert(center + vec({w,h}));
    P.insert(center + vec({w,-h}));
    P.insert(center + vec({-w,h}));
    P.insert(center + vec({-w,-h}));
    return P;
}
cnc::algo::geometry::ConvexPolygon cnc::algo::geometry::convexPrimitive::Circle(cnc::scalar radius, int rad, cnc::vec center)
{
    ConvexPolygon P;
    for (int i = 0;i<rad;i++){
        scalar th = i*M_PI*2./scalar(rad);
        P.insert(center + vec({std::cos(th),std::sin(th)})*radius);
    }
    return P;
}

cnc::algo::geometry::ConvexPolygon cnc::algo::geometry::convexPrimitive::Segment(const cnc::vec &a, const cnc::vec &b)
{
    ConvexPolygon P;
    P.insert(a);
    P.insert(b);
    return P;
}
