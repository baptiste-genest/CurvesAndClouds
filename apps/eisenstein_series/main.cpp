#include <curvesandcloud.h>

using namespace cnc;

cscalar lambert(const cscalar& q,uint a){
    if (std::abs(q) > 1.)
        std::cerr << "won't converge" << std::endl;

    cscalar sum = 0;
    auto qn = q;
    static constexpr uint N = 150;
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

cscalar img_by_matrix(const mat& A){
    cscalar z(A(0,0),A(0,1)),w(A(1,0),A(1,1));
    return w/z;
}

mat pow(const mat& A,scalar t){
    mat D(2);
    D(0,0) = std::pow(A(0,0),t);
    D(1,1) = std::pow(A(1,1),t);
    return D;
}

vec stereoproj(const cscalar& z1,const cscalar& z2){
    auto f = 1./(0.87-z2.imag());
    return linear_utils::vec2(z1.real()*f,z1.imag()*f);
}

cscalar brut_eisenstein(const cscalar& tau,int k){
    int N = 50;
    cscalar s = 0;
    for (int m = -N;m<=N;m++)
        for (int n = -N;n<=N;n++)
            if (m!=n)
                s += std::pow(cscalar(scalar(n)+scalar(m)*tau),-2*k);
    return s;
}

scalar m2(const cscalar& z){
    auto mz = std::abs(z);
    return mz*mz;
}

int main(int argc, char *argv[])
{
    mat U(2,2,{1,1,0,1.});
    mat V(2,2,{1,0,1,1.});
    auto A = U*V*V*U*U;
    auto ep = algo::get_2x2_eigenpaires(A);
    auto P = chaskal::build_basis(ep);
    auto Pi = algo::invert22(P);
    auto D = algo::diag(ep);
    std::cout << P*D*Pi << std::endl;
    std::cout << D << std::endl;
    auto z= img_by_matrix(A);
    auto period = std::log(std::abs(D(0,0)));
    auto ls = algo::calculus::get_lin_space(-1,1,1000);
    cloud knot;
    for (const auto& t : ls){
        auto z = img_by_matrix(P*pow(D,t)*Pi);
        auto G2 = g2(z);
        auto G3 = g3(z);
        auto k = newton(m2(G2),m2(G3));
        G2 = std::pow(k,-4)*G2;
        G3 = std::pow(k,-6)*G3;
        //std::cout << m2(G2)+m2(G3) << std::endl;
        knot.add_point(stereoproj(G2,G3));
    }
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 5;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);

    L->new_point_cloud(knot);

    w.show();
    return App.exec();
}
