#include <curvesandcloud.h>
#include <algorithm>

using namespace cnc;

using seg = std::pair<int,int>;
using hull = std::vector<seg>;

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

scalar Area(const point& a,const point& b,const point& c){
    return std::abs(algo::det22(b.first-a.first,c.first-a.first))*0.5;
}

struct Hyperplane{
    point a,b;
    scalar l2;
    vec d;
    Hyperplane(const point& x,const point& y) : a(x),b(y){
        l2 = a.first.distance2(b.first);
        d = (b.first-a.first).normalize();
    }
    vec proj(const vec& x) const {
        return a.first + d*(x-a.first).scalar_product(d);
    }
    scalar dist2(const point& x) const{
        return x.first.distance2(proj(x.first));
    }
    void addEdges(hull& h) const{
        h.push_back({a.second,b.second});
    }
    bool isVertex(const point& x) const{
        return x.second == a.second || x.second == b.second;
    }
    scalar bary(const point& x) const{
        return Area(a,b,x);
    }
    scalar dist2ToSeg(const point& x) const {
        auto p = proj(x.first);
        scalar d = p.distance2(x.first);
        scalar da = a.first.distance2(p);
        scalar db = b.first.distance2(p);
        if (da < l2 && db < l2)
            return d;
        return std::min(a.first.distance2(x.first),b.first.distance2(x.first));
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
    auto H1 = Hyperplane(H.a,F);
    auto H2 = Hyperplane(F,H.b);
    scalar AT = Area(H.a,H.b,F);
    auto rem = filter(X,[H,H1,H2,AT](const point& p){
        return (H.bary(p) + H1.bary(p) + H2.bary(p)) > AT+1e-3 && !H1.isVertex(p) && !H2.isVertex(p);
    });
    cloudpoint A,B;
    for (auto& p : rem){
        if (H1.dist2ToSeg(p) < H2.dist2ToSeg(p))
            A.push_back(p);
        else
            B.push_back(p);
    }
    expandHull(A,H1,R);
    expandHull(B,H2,R);
}

hull BestHull(const cloud& X){
    auto C = toCP(X);

    auto cmp = [](const point& a,const point& b){
        return a.first(0) < b.first(0);
    };
    auto LM = *std::min_element(C.begin(),C.end(),cmp);
    auto RM = *std::max_element(C.begin(),C.end(),cmp);

    Hyperplane H(LM,RM);

    cloudpoint A = filter(C,[LM,RM](const point& x){
        return algo::det22(x.first-LM.first,RM.first-LM.first) > 0 && (x.second != LM.second) && (x.second != RM.second);
    });
    cloudpoint B = filter(C,[LM,RM](const point& x){
        return algo::det22(x.first-LM.first,RM.first-LM.first) < 0 && (x.second != LM.second) && (x.second != RM.second);
    });
    hull R;
    expandHull(A,H,R);
    expandHull(B,H,R);
    return R;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* Layer = F->add_layer();


    if (false){
        auto C = algo::stat::random_var::sample_uniform_in_square(2,1,200);
        auto H = BestHull(C);

        range R(-2,2);
        auto E = Layer->add_euclidean_plane(R,R);
        std::vector<euclid::Point*> P;

        for (int i = 0;i<C.size();i++){
            P.push_back(E->add_object<euclid::Point>(C[i],6));
        }
        for (const auto& seg : H)
            E->add_object<euclid::Segment>(P[seg.first],P[seg.second]);

    }
    else {
        auto X = algo::stat::random_var::sample_uniform_in_square(1,1,10);
        cloud C;
        for (auto& x : X.points)
            C.add_point(vec({x(0),x(0)*x(0)}));
        auto H = BestHull(C);

        range R(-2,2);
        auto E = Layer->add_euclidean_plane(R,R);
        std::vector<euclid::Point*> P;

        for (const auto& x : C.points)
            P.push_back(E->add_object<euclid::Point>(x,6));

        for (const auto& seg : H)
            E->add_object<euclid::Segment>(P[seg.first],P[seg.second]);
    }

    w.show();
    return App.exec();
}
