#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::linear_utils;
using namespace cnc::euclid;


cmat U(2,2,{1,1,0,1.});
cmat V(2,2,{1,0,1,1.});

cmat words[4] = {U,V,V.invert(),U.invert()};

scalar ymax = 10;

namespace hyperbolic {

class idealPoint : public cscalar{
public:
    idealPoint(scalar x,scalar y){
        real(x);
        imag(y);
        isInfinity = false;
    }

    idealPoint(const cscalar& z){
        real(z.real());
        imag(z.imag());
        isInfinity = false;
    }

    idealPoint(bool i = false) : idealPoint(0,0){
        isInfinity = i;
    }
    scalar operator()(uint i) const {
        if (i)
            return imag();
        return real();
    }

    bool isInf() const {
        return isInfinity;
    }
private:
    bool isInfinity = false;
};

static const idealPoint inf(true);

using triangle = std::array<idealPoint,3>;

scalar eps = 1e-10;

scalar mod2(const cscalar& z1){
    auto m =std::abs(z1);
    return m*m;
}

idealPoint action(const cmat& A,const idealPoint& z){
    if (z.isInf()){
        if (std::abs(A(0,1)) < eps)
            return inf;
        return A(0,0)/A(0,1);
    }
    if (std::abs(z + A(1,1)/A(0,1)) < eps)
        return inf;
    return (A(0,0)*z+A(1,0))/(A(0,1)*z+A(1,1));
}

triangle action(const cmat& A,const triangle& T){
    auto NT = T;
    for (auto& x: NT)
        x = action(A,x);
    return NT;
}

scalar arg(const cscalar& z){
    auto th = std::atan2(z.imag(),z.real());
    if (th < 0)
        return 2*M_PI + th;
    return th;
}

void plotSide(EuclideanPlane* E,const idealPoint& x1,const idealPoint& x2){
    if (x2.isInf()){
        auto p1 = E->add_object<euclid::Point>(vec2(x1(0),x1(1)),0);
        auto p2 = E->add_object<euclid::Point>(vec2(x1(0),ymax),0);
        E->add_object<euclid::Segment>(p1,p2);
    }
    else if (x1.isInf()){
        auto p1 = E->add_object<euclid::Point>(vec2(x2(0),x2(1)),0);
        auto p2 = E->add_object<euclid::Point>(vec2(x2(0),ymax),0);
        E->add_object<euclid::Segment>(p1,p2);
    }
    else if (std::abs(x1.real() - x2.real()) < eps){
        auto p1 = E->add_object<euclid::Point>(vec2(x1(0),x1(1)),0);
        auto p2 = E->add_object<euclid::Point>(vec2(x2(0),x2(1)),0);
        E->add_object<euclid::Segment>(p1,p2);
    }
    else {
        vec x = vec2((mod2(x1)-mod2(x2))/(2*(x1.real()-x2.real())),0);
        scalar r = std::abs(x1-x(0));
        auto c1 = x1-x(0);auto a1 = arg(c1);
        auto c2 = x2-x(0);auto a2 = arg(c2);
        auto sa = std::min(a1,a2);
        auto as = std::max(a1,a2) - sa;
        E->add_object<euclid::CircleArc>(x,r,sa,as);
    }
}

void plot_triangle(const triangle& T,EuclideanPlane* E){
    for (uint i = 0;i<3;i++)
        plotSide(E,T[i],T[(i+1)%3]);
}

uint MAXITER = 7;
void plotSchotkyGroup(euclid::EuclideanPlane* E,const triangle& T,const cmat& A = chaskal::Id<cscalar>(2),int previous=-1,uint n = 0){
    if (n >= MAXITER)
        return;
    plot_triangle(T,E);
    for (int i = 0;i<4;i++){
        int j = 4-i;
        if (j != previous){
            auto NA = words[i]*A;
            plotSchotkyGroup(E,action(NA,T),NA,i,n+1);
        }
    }
}



}

namespace c {
cscalar i(0,1);
}

cmat P(2,2,{c::i,1.,1.,c::i});
auto iP = P.invert();

range build_range(scalar total_length,scalar center = 0){
    return {center - total_length*0.5,center+total_length*0.5};
}

int main(int argc, char *argv[])
{
    using namespace hyperbolic;

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    auto l = std::sqrt(1-0.25);
    triangle FD;
    FD[0] = idealPoint(-0.5,l);
    FD[1] = idealPoint(0.5,l);
    FD[2] = inf;


    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    scalar W = 3;
    range r = build_range(W);
    auto E = L->add_euclidean_plane(r,build_range(W,W*0.5-0.1));

    auto T1 = action(U,FD);
    plot_triangle(T1,E);
    auto T2 = action(V,FD);
    plot_triangle(T2,E);
    plotSchotkyGroup(E,FD);
    /*
    sl1->draw(E);
    sl2->draw(E);
    ct->draw(E);
    */

    w.show();
    return App.exec();
}
