#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::calculus;

using matfunction = std::function<mat(const vec&)>;
scalar a = 2.;
scalar b = 3.;

scalar sq(scalar x){
    return x*x;
}

scalar ellipse(const vec& x){
    return sq(x(0)/a) + sq(x(1)/b);
}

vec grad(const vec& x){
    return linear_utils::vec2(x(0)*2/sq(a),x(1)*2/sq(b));
}

mat hessian(const vec&){
    return mat(2,2,{2./sq(a),0,0,2./sq(b)});
}

std::vector<vec> levelset(const scalar_function& f,const matfunction& H,const vector_function& grad,const vec& x0,scalar dx,uint n){
    std::vector<vec> S(n);
    auto x = x0;
    for (uint i = 0;i<n;i++){
        S[i] = x;
        std::cout << f(x) << std::endl;
        auto Ha = H(x);
        auto ga = grad(x);
        auto h = cnc::algo::solve22(Ha,ga*(-2.));
        x += h;
        std::cout << x << std::endl;
    }
    return S;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 5;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);

    auto C = levelset(ellipse,hessian,grad,linear_utils::vec2(a,0),0.1,50);
    L->new_2D_curve(C,true);

    w.show();
    return App.exec();
}
