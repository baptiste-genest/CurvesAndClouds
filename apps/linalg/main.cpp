#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::calculus;

template<class T>
void print(const std::vector<T>& X){
    for (const T& x : X)
        std::cout << x << std::endl;
    std::cout << std::endl;
}

mat exp(const mat& M){
    mat X = chaskal::Id<scalar>(2);
    scalar ifac = 1.0;
    mat R = X;
    for (uint k = 1;k<50;k++){
        ifac /= scalar(k);
        X = M*X;
        R = R + X*ifac;
    }
    return R;
}

mat A(2,2,{1,0,0,2});
vec b({3.,1.});
mat P1(2,2,{0,1,-1,0});
mat M1 = P1*A*2;
vec B1 = P1*b;
vec bp = M1.solve(B1);

scalar F(const vec& x) {
    return (x.transpose()*A*x + x.transpose()*b)(0,0);
}

scalar get_initial_condition(const vec& x){
    scalar f = F(x-bp)-1;
    return f*f;
}

int main(int argc, char *argv[])
{
    scalar a = 4,b=3,c=-2;
    mat P(2,2,{a,b/2,b/2,c});
    auto eigen = P.lanczos();
    for (auto& e : eigen)
        std::cout << e.vector << std::endl;
    return 0;
    mat P1(2,2,{0,1,-1,0});
    mat P2 = -P1;
    mat M2 = P2*A;
    vec y0;
    y0 = algo::calculus::optimization::gradient_descent(get_initial_condition,vec(2),0.2,2000);
    scalar t = 0,dt = 0.1;
    uint N = 40;
    std::vector<vec> O1(N),O2(N);
    std::cout <<"initial: " << F(y0-bp) << std::endl;
    for (uint k = 0;k<N;k++){
        mat E = exp(M1*t);
        O1[k] = E*y0 - bp;
        t += dt;
    }
    QApplication app(argc,argv);
    Plot_window w; w.resize(500,500);

    Plot_tab* T = w.add_tab("my first tab");
    Plot_frame* F= T->add_frame();
    Plot_layer* L = F->add_layer();

    L->new_2D_curve(O1)->fix_plot_in_rect(0,0,10);
    //L->new_2D_curve(O2)->fix_plot_in_rect(0,0,5);
    w.show();
    return app.exec();
}
