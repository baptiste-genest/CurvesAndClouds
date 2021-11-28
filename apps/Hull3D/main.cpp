#include <curvesandcloud.h>
#include <algorithm>

using namespace cnc;

using facet = std::array<int,3>;
using hull = std::vector<facet>;

using point = std::pair<vec,int>;
using cloudpoint = std::vector<point>;

cloudpoint toCP(const cloud& X){
    cloudpoint C;
    for (uint k = 0;k<X.size();k++)
        C.push_back({X[k],k});
    return C;
}

using predicate = std::function<bool(const point& x)>;

cloudpoint filter(const cloudpoint& C,const predicate& P){
    cloudpoint F;
    for (const auto& x : C)
        if (P(x))
            F.push_back(x);
    return F;
}

scalar Area(const point& a,const point& b,const point& c,const point& d){
    return std::abs(algo::det33(b.first-a.first,c.first-a.first,d.first-a.first))*0.5;
}
scalar Area(const vec& a,const vec& b,const vec& c,const vec& d){
    return std::abs(algo::det33(b-a,c-a,d-a))*0.5;
}

struct seg{
    vec a,b;
    scalar l2;
    vec d;
    seg(const vec& x,const vec& y) : a(x),b(y){
        l2 = a.distance2(b);
        d = (b-a).normalize();
    }
    vec proj(const vec& x) const {
        return a + d*(x-a).scalar_product(d);
    }
    scalar dist2(const point& x) const{
        return x.first.distance2(proj(x.first));
    }
    scalar dist2ToSeg(const point& x) const {
        auto p = proj(x.first);
        scalar d = p.distance2(x.first);
        scalar da = a.distance2(p);
        scalar db = b.distance2(p);
        if (da < l2 && db < l2)
            return d;
        return std::min(a.distance2(x.first),b.distance2(x.first));
    }
};

struct Hyperplane{
    point a,b,c;
    mat P;
    vec O;
    seg s1,s2,s3;
    Hyperplane(const point& x,const point& y,const point& z) : a(x),b(y),c(z),s1(x.first,y.first),s2(x.first,z.first),s3(y.first,z.first){
        mat S(3,2);
        vec U = y.first - x.first;
        vec V = z.first - x.first;
        for (uint j = 0;j<3;j++)
            S(0,j) = U(j);
        for (uint j = 0;j<3;j++)
            S(1,j) = V(j);
        P = S*(S.transpose()*S).invert()*S.transpose();
        O = x.first - P*x.first;
    }
    vec proj(const vec& x) const {
        return P*x+O;
    }
    scalar dist2(const point& x) const{
        return x.first.distance2(proj(x.first));
    }
    void addEdges(hull& h) const{
        h.push_back({a.second,b.second,c.second});
    }
    bool isVertex(const point& x) const{
        return x.second == a.second || x.second == b.second || x.second == c.second;
    }
    scalar bary(const point& x) const{
        return Area(a,b,c,x);
    }
    scalar bary(const vec& x) const{
        return Area(a.first,b.first,c.first,x);
    }
    scalar dist2ToFace(const point& x) const {
        using namespace algo;
        auto p = proj(x.first);
        vec A = a.first-x.first;
        vec B = b.first-x.first;
        vec C = c.first-x.first;
        vec u = cross(B,C);
        vec v = cross(C,A);
        vec w = cross(A,B);
        if (u.scalar_product(v) < 0 || u.scalar_product(w) < 0){
            std::vector<scalar> d = {s1.dist2ToSeg(x),s2.dist2ToSeg(x),s3.dist2ToSeg(x)};
            return *std::min_element(d.begin(),d.end());
        }
        return p.distance2(x.first);
    }
};


void expandHull(const cloudpoint& X,const Hyperplane& H,hull& R){
    if (X.size() == 0){
        H.addEdges(R);
        return;
    }
    auto F = *std::max_element(X.begin(),X.end(),[H](const point& x,const point& y){
        return H.dist2(x) < H.dist2(y);
    });
    auto H1 = Hyperplane(H.a,H.b,F);
    auto H2 = Hyperplane(H.b,H.c,F);
    auto H3 = Hyperplane(H.a,H.c,F);
    scalar AT = Area(H.a,H.b,H.c,F);
    auto rem = filter(X,[H,H1,H2,H3,AT](const point& p){
        return (H.bary(p) + H1.bary(p) + H2.bary(p) + H3.bary(p)) > AT+1e-3 && !H1.isVertex(p) && !H2.isVertex(p) && !H3.isVertex(p);
    });
    cloudpoint C[3];
    for (auto& p : rem){
        std::vector<scalar> D = {H1.dist2ToFace(p),H2.dist2ToFace(p),H3.dist2ToFace(p)};
        auto M = std::min_element(D.begin(),D.end());
        C[M-D.begin()].push_back(p);
    }
    expandHull(C[0],H1,R);
    expandHull(C[1],H2,R);
    expandHull(C[2],H3,R);
}

hull BestHull(const cloud& X){
    auto C = toCP(X);

    auto cmp1 = [](const point& a,const point& b){
        return a.first(0) < b.first(0);
    };
    auto cmp2 = [](const point& a,const point& b){
        return a.first(1) < b.first(1);
    };
    auto LM = *std::min_element(C.begin(),C.end(),cmp1);
    auto RM = *std::max_element(C.begin(),C.end(),cmp1);
    auto ZM = *std::max_element(C.begin(),C.end(),cmp2);
    if (ZM.second == LM.second ||ZM.second == RM.second)
        ZM = *std::min_element(C.begin(),C.end(),cmp2);

    Hyperplane H(LM,RM,ZM);

    cloudpoint A = filter(C,[LM,RM,ZM](const point& x){
        return algo::det33(x.first-LM.first,RM.first-LM.first,ZM.first-LM.first) > 0 && (x.second != LM.second) && (x.second != RM.second) && (x.second != ZM.second);
    });
    cloudpoint B = filter(C,[LM,RM,ZM](const point& x){
        return algo::det33(x.first-LM.first,RM.first-LM.first,ZM.first-LM.first) < 0 && (x.second != LM.second) && (x.second != RM.second) && (x.second != ZM.second);
    });
    hull R;
    expandHull(A,H,R);
    expandHull(B,H,R);
    return R;
}

mat Rot(scalar th){
    mat R(3);
    R(0,0) = cos(th);
    R(2,0) = sin(th);
    R(1,1) = 1;
    R(0,2) = -sin(th);
    R(2,2) = cos(th);
    return R;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    auto th = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta");

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* Layer = F->add_layer();

    auto Pr = [th] (const vec& y){
        vec x = Rot(th)*y;
        return vec({x(0),x(1)});
    };

    if (true){
        auto C = algo::stat::random_var::sample_uniform_in_square(3,1,20);
        auto H = BestHull(C);

        range R(-2,2);
        auto E = Layer->add_euclidean_plane(R,R);
        E->set_dynamic();
        std::vector<euclid::Point*> P;

        for (const auto& x : C.points)
            P.push_back(E->add_object<euclid::Point>([Pr,x](){
                return Pr(x);
            },6));

        for (const auto& face : H){
            for (uint i =0;i<3;i++)
                E->add_object<euclid::Segment>(P[face[i]],P[face[(i+1)%3]]);
        }
    }
    else {
        auto X = algo::stat::random_var::sample_uniform_in_square(2,1,10);
        cloud C;
        for (auto& x : X.points)
            C.add_point(vec({x(0),x(1),x(0)*x(0) + x(1)*x(1)}));
        auto H = BestHull(C);

        range R(-2,2);
        auto E = Layer->add_euclidean_plane(R,R);
        std::vector<euclid::Point*> P;

        for (const auto& x : C.points)
            P.push_back(E->add_object<euclid::Point>(Pr(x),6));

        for (const auto& face : H){
            for (uint i =0;i<3;i++)
                E->add_object<euclid::Segment>(P[face[i]],P[face[(i+1)%3]]);
        }
    }

    w.show();
    return App.exec();
}
