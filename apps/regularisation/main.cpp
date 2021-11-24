#include <curvesandcloud.h>

using namespace cnc;

scalar f(scalar x){
    scalar r = 0;
    static scalar a = 0.5,b=2;
    scalar an = 1,bn = 1;
    for (uint k = 0;k<15;k++){
        r += an*cos(M_PI*bn*x);
        an *= a;
        bn *= b;
    }
    return r;
}

scalar convolute(const algo::calculus::scalar_function_1D& f,const algo::calculus::scalar_function_1D& g,range X,scalar x){
    auto Tc = algo::calculus::get_tchebytchev_points(20,X);
    auto S = algo::calculus::integration_scheme::simpson([f,g,x] (scalar y){
        return f(y)*g(x-y);
    });
    return algo::calculus::integrate(S,Tc);
}

int main(int argc, char *argv[])
{
    auto K = [] (scalar x) {
        if (std::abs(x) > 1)
            return 0.;
        return std::exp(-1/(1-x*x));
    };

    auto S = algo::calculus::integration_scheme::simpson(K);

    auto Tc = algo::calculus::get_tchebytchev_points(20,{-1,1});

    scalar I = algo::calculus::integrate(S,Tc);

    auto KN = [K,I] (scalar x){
        return K(x)/I;
    };



    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("convolution");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    auto eps = w.add_mutable_scalar_by_cursor({0.,3.},"epsilon",200);
    auto a = w.add_mutable_scalar_by_cursor({0.,1.},"a",100);
    auto b = w.add_mutable_scalar_by_cursor({0.,5.},"b",100);

    auto K_eps = [eps,KN] (scalar x) {
        return KN(x/eps)/eps;
    };

    range R = {-4,4};

    auto W = [a,b] (scalar x){
        scalar r = 0;
        scalar an = 1,bn = 1;
        for (uint k = 0;k<15;k++){
            r += an*cos(M_PI*bn*x);
            an *= a;
            bn *= b;
        }
        return r;
    };

    auto reg = [eps,K_eps,W] (scalar x) {
        return convolute(K_eps,W,{-eps,eps},x);
    };

    //L->new_function_plot(K_eps,R)->set_dynamic();
    //L->new_function_plot(f,R);
    L->new_function_plot(W,R)->set_dynamic();
    L->new_function_plot(reg,R)->set_dynamic();

    w.show();
    return App.exec();
}
