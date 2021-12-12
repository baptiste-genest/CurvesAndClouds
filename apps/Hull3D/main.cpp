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

bool SameSide(const vec& v1,const vec& v2,const vec& v3,const vec& v4,const vec& p)
{
    vec normal = algo::cross(v2 - v1, v3 - v1);
    scalar dotV4 = algo::dot(normal, v4 - v1);
    scalar dotP = algo::dot(normal, p - v1);
    return dotV4 * dotP > 0;
}

bool PointInTetrahedron(const vec& v1,const vec& v2,const vec& v3,const vec& v4,const vec& p)
{
    return SameSide(v1, v2, v3, v4, p) &&
            SameSide(v2, v3, v4, v1, p) &&
            SameSide(v3, v4, v1, v2, p) &&
            SameSide(v4, v1, v2, v3, p);
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
        vec A = a.first-p;
        vec B = b.first-p;
        vec C = c.first-p;
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
    //scalar AT = Area(H.a,H.b,H.c,F);
    std::vector<vec> C(4);
    C[0] = H.a.first;
    C[1] = H.b.first;
    C[2] = H.c.first;
    C[3] = F.first;
    auto rem = filter(X,[C,H1,H2,H3](const point& p){
        return !PointInTetrahedron(C[0],C[1],C[2],C[3],p.first) && !H1.isVertex(p) && !H2.isVertex(p) && !H3.isVertex(p);
    });
    cloudpoint C2[3];
    for (const auto& p : rem){
        std::vector<scalar> D = {H1.dist2ToFace(p),H2.dist2ToFace(p),H3.dist2ToFace(p)};
        auto M = std::min_element(D.begin(),D.end());
        C2[M-D.begin()].push_back(p);
    }
    expandHull(C2[0],H1,R);
    /*
    expandHull(C2[1],H2,R);
    expandHull(C2[2],H3,R);
    */
}

scalar slope(const vec& A,const vec& B){
    return (B(1)-A(1))/(B(0)-A(0));
}

hull get_first_points_on_hull(const cloud& X,std::vector<point>& F){
    auto C = toCP(X);
    auto cmp1 = [](const point& a,const point& b){
        return a.first(0) < b.first(0);
    };
    auto LM = *std::min_element(C.begin(),C.end(),cmp1);
    auto cmp2 = [LM](const point& a,const point& b){
        if (LM.second == a.second)
            return true;
        if (LM.second == b.second)
            return false;
        if (slope(LM.first,a.first) < slope(LM.first,b.first))
            return true;
        return false;
    };
    auto TM = *std::max_element(C.begin(),C.end(),cmp2);
    auto x = LM.first, y = TM.first;
    auto cmp3 = [x,y](const point& a,const point& b){
        vec na = algo::cross(y-x,a.first-x).normalize();
        vec nb = algo::cross(y-x,b.first-x).normalize();
        return na(0) < nb(0);
    };
    auto NM = *std::max_element(C.begin(),C.end(),cmp3);
    Hyperplane H(LM,TM,NM);
    hull R;
    C = filter(C,[LM,TM,NM] (const point& p){
        return p.second != LM.second && p.second != TM.second && p.second != NM.second;
    });
    F = {LM,TM,NM};
    expandHull(C,H,R);
    return R;
}

hull BestHull(const cloud& X,std::vector<int>& L,std::vector<int>& U,int i = 0){
    auto C = toCP(X);

    auto cmp1 = [i](const point& a,const point& b){
        return a.first(i) < b.first(i);
    };
    auto cmp2 = [i](const point& a,const point& b){
        return a.first((i+1)%3) < b.first((i+1)%3);
    };
    auto LM = *std::min_element(C.begin(),C.end(),cmp1);
    auto RM = *std::max_element(C.begin(),C.end(),cmp1);
    auto ZM = *std::max_element(C.begin(),C.end(),cmp2);
    if (ZM.second == LM.second ||ZM.second == RM.second)
        ZM = *std::min_element(C.begin(),C.end(),cmp2);
    std::cout << LM.second << ' ' << RM.second << ' ' << ZM.second << std::endl;

    Hyperplane H(LM,RM,ZM);

    cloudpoint A = filter(C,[LM,RM,ZM](const point& x){
        return algo::det33(x.first-LM.first,RM.first-LM.first,ZM.first-LM.first) > 0 && (x.second != LM.second) && (x.second != RM.second) && (x.second != ZM.second);
    });
    cloudpoint B = filter(C,[LM,RM,ZM](const point& x){
        return algo::det33(x.first-LM.first,RM.first-LM.first,ZM.first-LM.first) < 0 && (x.second != LM.second) && (x.second != RM.second) && (x.second != ZM.second);
    });
    for (const auto& x : A)
        L.push_back(x.second);
    for (const auto& x : B)
        U.push_back(x.second);
    hull R;
    expandHull(A,H,R);
    //expandHull(B,H,R);
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
    /*
    PlotFrame* F= T->add_frame();
    PlotLayer* Layer = F->add_layer();
    */

    auto Pr = [th] (const vec& y){
        vec x = Rot(th)*y;
        return vec({x(0),x(1)});
    };

    if (false){
        auto C = algo::stat::random_var::sample_uniform_in_square(3,1,20);
        for (int i = 0;i<2;i++){
            PlotFrame* F= T->add_frame();
            PlotLayer* Layer = F->add_layer();
            std::vector<int> U,L;
            auto H = BestHull(C,U,L,i);
            range R(-2,2);
            auto E = Layer->add_euclidean_plane(R,R);
            E->set_dynamic();
            std::vector<euclid::Point*> P;

            for (int i = 0;i<C.size();i++){
                auto x = C[i];
                P.push_back(E->add_object<euclid::Point>([Pr,x](){
                    return Pr(x);
                },4));
                if (std::find(U.begin(),U.end(),i) != U.end())
                    P.back()->set_color(QColorConstants::Red);
                if (std::find(L.begin(),L.end(),i) != L.end())
                    P.back()->set_color(QColorConstants::Blue);
            }

            for (const auto& face : H){
                for (uint i =0;i<3;i++)
                    E->add_object<euclid::Segment>(P[face[i]],P[face[(i+1)%3]]);
            }
        }
    }
    else {
        auto C = algo::stat::random_var::sample_uniform_in_square(3,1,20);
        PlotFrame* F= T->add_frame();
        PlotLayer* Layer = F->add_layer();
        std::vector<point> B;
        auto H = get_first_points_on_hull(C,B);
        range R(-2,2);
        auto E = Layer->add_euclidean_plane(R,R);
        E->set_dynamic();
        std::vector<euclid::Point*> P;

        for (uint i = 0;i<C.size();i++){
            auto x = C[i];
            P.push_back(E->add_object<euclid::Point>([Pr,x](){
                return Pr(x);
            },4));
            bool isf = false;
            for (auto p : B)
                isf ^= p.second == int(i);
            if (isf)
                P.back()->set_color(QColorConstants::Red);

        }

        for (const auto& face : H){
            for (uint i =0;i<3;i++)
                E->add_object<euclid::Segment>(P[face[i]],P[face[(i+1)%3]]);
        }
    }
    /*
    else if (true){
    uint N = 8000;
    auto C = toCP(algo::stat::random_var::sample_uniform_in_square(3,1,4));
    auto X = toCP(algo::stat::random_var::sample_uniform_in_square(3,1.4,N));
    auto H = Hyperplane(C[0],C[1],C[2]);
    auto H1 = Hyperplane(C[0],C[1],C[3]);
    auto H2 = Hyperplane(C[0],C[2],C[3]);
    auto H3 = Hyperplane(C[1],C[2],C[3]);

    range R(-2,2);
    auto E = Layer->add_euclidean_plane(R,R);
    E->set_dynamic();
    std::vector<euclid::Point*> P;

    int r= 4;
    for (const auto& x : C ){
    P.push_back(E->add_object<euclid::Point>([Pr,x](){
    return Pr(x.first);
    },r));
    }
    for (uint i =0;i<4;i++){
    E->add_object<euclid::Segment>(P[i],P[(i+1)%4]);
    }
    //scalar AT = Area(H.a,H.b,H.c,C[4]);
    auto rem = filter(X,[C](const point& p){
    return PointInTetrahedron(C[0].first,C[1].first,C[2].first,C[3].first,p.first);
    });
    for (auto& x : rem){
    auto p = x.first;
    P.push_back(E->add_object<euclid::Point>([Pr,p](){
    return Pr(p);
    },2));
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
    */

    w.show();
    return App.exec();
}
