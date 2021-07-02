#include <curvesandcloud.h>

using namespace cnc;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    scalar r = 3.;
    range X{-r,r};
    auto a = w.add_mutable_scalar_by_cursor(X,"a");
    auto b = w.add_mutable_scalar_by_cursor(X,"b");
    auto c = w.add_mutable_scalar_by_cursor(X,"c");
    auto d = w.add_mutable_scalar_by_cursor(X,"d");
    auto e = w.add_mutable_scalar_by_cursor(X,"d");
    auto g = w.add_mutable_scalar_by_cursor(X,"d");

    auto f = [a,b,c,d,e,g] (complex_scalar z){
        complex_scalar z1(a,b),z2(c,d),z3(e,g);
        return (z-z1)*(z-z2)*(z-z3);
    };
    auto df = [a,b,c,d,e,g] (complex_scalar z){
        complex_scalar z1(a,b),z2(c,d),z3(e,g);
        return 3.*z*z - 2.*z*(z1+z2+z3) + z1*z2 + z1*z3 + z2*z3;
    };

    auto newton = [f,df] (complex_scalar z){
        uint iter = 0;
        while (std::abs(f(z)) > 1e-3 && iter < 20){
            iter++;
            z -= f(z)/df(z);
        }
        return z;
    };

    auto P = L->new_complex_function_plot(newton,X,X);
    P->set_dynamic();
    P->set_resolution(200);

    w.show();
    return App.exec();
}
