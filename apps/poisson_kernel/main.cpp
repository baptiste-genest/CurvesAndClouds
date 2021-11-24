#include <curvesandcloud.h>

using namespace cnc;
using namespace algo::calculus;

using cnodes = std::vector<complex_scalar>;

scalar f(scalar x){
    return cos(x)*x;
}

scalar_function_1D I;

scalar poisson_kernel(scalar x,scalar y){
    return (1.0/M_PI)*y/(x*x+y*y);
}

cnodes hadamar_prod(const cnodes& A,const cnodes& B){
    cnodes P(A.size());
    for (uint k = 0;k<A.size();k++)
        P[k] = A[k]*B[k];
    return P;
}

nodes convolute(const nodes& A,const nodes& B){
    return R_iFFT(hadamar_prod(FFT(A),FFT(B)));
}


int main(int argc, char *argv[])
{
    range r = {-3,3};
    auto X = algo::calculus::get_lin_space(r,1024);

    auto fD = algo::calculus::sample(f,X);
    I = build_1D_linear_interpolator(X,fD);
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);
    auto l = w.add_mutable_scalar_by_cursor({0,3},"lambda");

    PlotTab* T = w.add_tab("Poisson Kernel");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    /*
    auto Pd = sample([l] (scalar x){	return poisson_kernel(x,l);	},X);
    I = build_1D_linear_interpolator(X,convolute(Pd,fD));
    */
    algo::calculus::scalar_function_1D plot = [fD,l,X] (scalar x) {
        if (l.time_since_last_update_in_milliseconds() < 2){
            auto Pd = sample([l] (scalar x){	return poisson_kernel(x,l);	},X);
            I = build_1D_linear_interpolator(X,convolute(Pd,fD));
        }
        return I(x);
    };

    auto p = L->new_function_plot(plot,r);
    p->set_dynamic();


    w.show();
    return App.exec();
}
