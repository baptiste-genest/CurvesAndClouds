#include <curvesandcloud.h>
#include "modular.h"
#include "stereoproj.h"

using namespace cnc;
using namespace cnc::symbolic;

std::vector<cscalar> img_by_matrix(const mat& A){
    cscalar z(A(0,0),A(0,1)),w(A(1,0),A(1,1));
    return {w/z,z};
}

mat pow(const mat& A,scalar t){
    mat D(2);
    D(0,0) = std::pow(A(0,0),t);
    D(1,1) = std::pow(A(1,1),t);
    return D;
}

cscalar brut_eisenstein(const mat& A,int k){
    int N = 20;
    cscalar s = 0;
    for (int m = -N;m<=N;m++)
        for (int n = -N;n<=N;n++)
            if (m!=n){
                vec zv = A*linear_utils::vec2(m,n);
                cscalar z(zv(0),zv(1));
                s += std::pow(z,-2*k);
            }
    return s;
}

scalar m2(const cscalar& z){
    auto mz = std::abs(z);
    return mz*mz;
}

cscalar fast_pow(const cscalar& z,int n){
    if (n < 0)
        return fast_pow(scalar(1.)/z,-n);
    if (n == 0)
        return 1.;
    if (n == 1)
        return z;
    auto zp = fast_pow(z,n/2);
    auto r = zp*zp;
    if (n%2)
        r *= z;
    return r;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    /*
    generate_trefoil_knot();
    return 0;
    */

    //mat A= genMat(6);
    mat A(2,2,{13,8,21,13});
    auto ep = algo::get_2x2_eigenpaires(A);
    auto P = chaskal::build_basis(ep);
    auto Pi = algo::invert22(P);
    auto D = algo::diag(ep);
    std::cout << P*D*Pi << std::endl;
    std::cout << D << std::endl;

    auto period = std::max(std::log(std::abs(D(0,0))),std::log(std::abs(D(1,1))));
    std::cout << period << std::endl;
    auto ls = algo::calculus::get_lin_space(0,period,2000);
    cloud knot;
    uint i = 0;
    for (const auto& t : ls){
        //std::cout << i << " over " << 200 << std::endl;
        i++;
        auto phi = P*algo::diag({std::exp(t),std::exp(-t)})*Pi;
        auto phiB = img_by_matrix(phi);

        const cscalar& tau = phiB[0];
        const cscalar& z= phiB[1];

        auto G2 = g2(tau)*fast_pow(z,-4);
        auto G3 = g3(tau)*fast_pow(z,-6);

        auto k = newton(m2(G2),m2(G3));
        G2 = std::pow(k,-4)*G2;
        G3 = std::pow(k,-6)*G3;
        auto X = ctov(G2,G3);
        knot.add_point(X);
    }
    //std::cout << P*algo::diag({std::exp(period),std::exp(-period)})*Pi << std::endl;

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 3;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);
    auto E = L->add_euclidean_plane(r,r);

    scalar eps = 0.001;
    auto trefoil = generate_trefoil_knot().subsample(eps);
    knot = knot.subsample(eps);

    auto th = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta");
    auto th2 = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta2");
    auto th3 = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta3");
    auto zoom = w.add_mutable_scalar_by_cursor({0,4},"zoom");
    auto pole = w.add_mutable_scalar_by_cursor({-4,4},"stereo pole");

    /*
    auto Pr = [th,pole,zoom] (const vec& y){
        vec x = algo::geometry::Ry(th)*standard_stereoproj(y,pole)*zoom;
        return vec({x(0),x(1)});
    };
    */
    auto Pr = [th,th2,th3,pole,zoom] (const vec& y){
        vec x = algo::geometry::Ry(th)*standard_stereoproj(y,pole)*zoom;
        return vec({x(0),x(1)});
    };

    for (const auto& x : knot.points){
        E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        })->set_color(QColorConstants::Red);
    }
    for (const auto& x : trefoil.points)
        E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        });
    /*
    for (const auto& angle : algo::calculus::get_lin_space(0,2*M_PI,100)){
        vec x(4);
        x(0) = cos(angle);
        x(1) = sin(angle);
        E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        })->set_color(QColorConstants::Green);
    }
    for (const auto& angle : algo::calculus::get_lin_space(0,2*M_PI,100)){
        vec x(4);
        x(2) = cos(angle);
        x(3) = sin(angle);
        E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        })->set_color(QColorConstants::Blue);
    }
    */

    w.show();
    return App.exec();
}
