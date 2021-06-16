#include <curvesandcloud.h>

using std::cout;
using std::endl;

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::calculus;

struct linear_fourier_series{

    scalar T;
    uint N;
    cmat polynomial_eval;
    cvec P;
    int order;

    std::vector<complex_scalar> cnp,cnn;
    scalar c0;

    linear_fourier_series(const vec& p,scalar _T,int o) : T(_T) , order(o) {
        N = p.rowNum();
        polynomial_eval = cmat(1,N);
        scalar t = T;
        for (uint k = 1;k<N;k++){
            polynomial_eval(N-k-1,0) = t;
            t *= _T;
        }
        std::cout << polynomial_eval << endl;
        P = cvec(N);
        for (uint k = 0;k<N;k++)
            P(k).real(p(k));
        cnp.resize(o);
        cnn.resize(o);
        compute_cn();
    }

    cmat get_Fd(int n) const {
        cmat Fd(N,N);
        scalar omega = 2*n*M_PI/T;
        for (uint k = 0;k<N;k++){
            Fd(k,k).imag(-omega);
            if (k)
                Fd(k-1,k).real(N-k);
        }
        return Fd;
    }

    void compute_cn() {
        c0 = 0;
        for (uint k = 0;k<N;k++)
            c0 += P(k).real()*std::pow(T,N-k-1)/((scalar)N-k);
        for (int k = 1;k<order;k++){
            cnp[k-1] = (polynomial_eval*get_Fd(k).lower_solve(P))(0,0) * 1./T;
            cnn[k-1] = (polynomial_eval*get_Fd(-k).lower_solve(P))(0,0) * 1./T;
        }
    }

    scalar evaluate(scalar t) const{
        scalar R = c0;
        for (int k = 1;k<=order;k++){
            scalar omega = 2*k*M_PI/T;
            R += (cnp[k-1] * std::polar(1.,omega*t) + cnn[k-1] * std::polar(1.,-omega*t)).real();
        }
        return R;
    }
};

complex_scalar hermitian_product(cvec w1,cvec w2){
    complex_scalar R = 0;
    for (uint k = 0;k<w1.rowNum();k++)
        R += w1(k)*std::conj(w2(k));
    return R;
}

int main(int argc, char *argv[])
{
    cvec w1({algo::stat::random_var::random_complex_scalar(-1,1),algo::stat::random_var::random_complex_scalar(-1,1)});
    //cvec w2({algo::stat::random_var::random_complex_scalar(-1,1),algo::stat::random_var::random_complex_scalar(-1,1)});
    cmat A(2,2);
    A(0,0).imag(-1);
    A(1,0) = complex_scalar(2,1);
    A(0,1) = complex_scalar(-2,1);
    std::cout << A << std::endl;
    std::cout << w1 << std::endl;
    std::cout << hermitian_product(w1,A*w1) << std::endl;
    return 0;
    vec p({1,-5,6,0});
    linear_fourier_series LFS(p,3,3);

    QApplication app(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    range t{0,3};
    GridLayer* L = F->add_grid_layer(t,{-3,3});
    L->new_function_plot([LFS](scalar x){return LFS.evaluate(x);},t);
    L->new_function_plot([p](scalar x){
        scalar X = 1.;
        uint n = p.rowNum();
        scalar eval = 0;
        for (uint k = 0;k<n;k++){
            eval += p(n-k-1)*X;
            X *= x;
        }
        return eval;
    },t);

    w.show();
    return app.exec();
}
