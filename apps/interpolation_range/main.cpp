#include <curvesandcloud.h>

using namespace cnc;
using namespace algo::calculus;

uint N = 5;

scalar f(scalar x) {
    return x*exp(-x*x*4);
}

scalar PI(scalar x,const nodes& X) {
    scalar R = 1;
    for (auto a : X)
        R *= a-x;
    return R;
}

property_2D build_property(const nodes& X,const lagrange_interpolator& L){
    scalar ifac = 1.;
    for (uint k = 1;k<=N;k++)
        ifac /= k;
    scalar M3 = 1.;

    return [X,L,ifac,M3] (scalar x,scalar y) {
        return std::abs(y-f(x)) < 100*std::abs(PI(x,X)*ifac*M3);
    };
}

int main(int argc, char *argv[])
{
    range X{-1,1};
    nodes x = get_lin_space(X,(int)N);

    lagrange_interpolator I(x,sample(f,x));

    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    L->new_function_plot(f,X);
    L->new_function_plot(I,X);

    auto P = build_property(x,I);
    L->new_boolean_field(P,X,X);

    w.show();
    return a.exec();
}
