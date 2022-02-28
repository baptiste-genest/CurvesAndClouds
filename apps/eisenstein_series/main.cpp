#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::symbolic;

cscalar lambert(const cscalar& q,uint a){
    if (std::abs(q) > 1.)
        std::cerr << "won't converge" << std::endl;

    cscalar sum = 0;
    auto qn = q;
    static constexpr uint N = 500;
    for (uint n = 1;n<N;n++){
        sum += std::pow(n,a)*qn/(1.-qn);
        qn = qn*q;
    }
    return sum;
}

scalar f(scalar x,scalar a,scalar b){
    return x*x*a + x*x*x*b -1.;
}
scalar df(scalar x,scalar a,scalar b){
    return 2.*x*a + 3.*x*x*b;
}

scalar newton(scalar a,scalar b){
    scalar x = 1.;
    while (std::abs(f(x,a,b)) > 1e-5)
        x -= f(x,a,b)/df(x,a,b);
    return std::pow(x,-0.25);
}

cscalar g2(const cscalar& tau){
    static const scalar factor = 60.*std::pow(M_PI,4)/45;
    cscalar q = std::exp(cscalar(0,2)*M_PI*tau);
    return factor*(1.+240.*lambert(q,3));
}

cscalar g3(const cscalar& tau){
    static const scalar factor = 140.*2*std::pow(M_PI,6)/945;
    cscalar q = std::exp(cscalar(0,2)*M_PI*tau);
    return factor*(1.-504.*lambert(q,5));
}

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

vec stereoproj(const cscalar& z1,const cscalar& z2){
    auto f = 1./(1.-z2.imag());
    return linear_utils::vec2(z1.real()*f,z1.imag()*f);
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

mat genMat(uint length){
    mat U(2,2,{1,1,0,1.});
    mat V(2,2,{1,0,1,1.});
    mat A(2,2,{1,0,0,1.});
    for (uint n = 0;n<length;n++)
        A = A * (rand()%2 ? U : V);
    return A;
}

Variable Z[4];
ValuationSystem ValuateVector(const cscalar& z,const cscalar& w){
    ValuationSystem V = {Z[0] == z.real(),
                         Z[1] == z.imag(),
                         Z[2] == w.real(),
                         Z[3] == w.imag()};
    return V;
}

cloud generate_trefoil_knot()
{
    cscalar I(0.,1.);
    const auto& a = Z[0];
    const auto& b = Z[1];
    const auto& c = Z[2];
    const auto& d = Z[3];
    Expression knot = pow(a+b*I,3)-27.*pow(c+d*I,2);

    svec G(4);
    for (uint j = 0;j<4;j++)
        G(j) = knot.differentiate(Z[j]);

    svec grads[2] = {svec(4),svec(4)};
    for (uint j= 0;j<4;j++){
        grads[0](j) = Re(G(j));
        grads[1](j) = Im(G(j));
    }
    std::cout << grads[0].print() << std::endl;
    std::cout << grads[1].print() << std::endl;
}

int main(int argc, char *argv[])
{
    generate_trefoil_knot();
    return 0;

    mat A= genMat(4);
    auto ep = algo::get_2x2_eigenpaires(A);
    auto P = chaskal::build_basis(ep);
    auto Pi = algo::invert22(P);
    auto D = algo::diag(ep);
    std::cout << P*D*Pi << std::endl;
    std::cout << D << std::endl;

    auto period = std::max(std::log(std::abs(D(0,0))),std::log(std::abs(D(1,1))));
    std::cout << period << std::endl;
    auto ls = algo::calculus::get_lin_space(0,period,1000);
    cloud knot;
    for (const auto& t : ls){
        auto phi = P*algo::diag({std::exp(t),std::exp(-t)})*Pi;
        auto phiB = img_by_matrix(phi);

        const cscalar& tau = phiB[0];
        const cscalar& z= phiB[1];

        auto G2 = g2(tau)/std::pow(z,4);
        auto G3 = g3(tau)/std::pow(z,6);

        auto k = newton(m2(G2),m2(G3));
        //std::cout << k << ' ' << 1./std::sqrt(m2(G2)+m2(G3)) << std::endl;
        G2 = std::pow(k,-4)*G2;
        G3 = std::pow(k,-6)*G3;
        knot.add_point(stereoproj(G2,G3));
    }

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 3;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);

    L->new_point_cloud(knot);

    w.show();
    return App.exec();
}
