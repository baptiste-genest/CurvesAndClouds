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

int main(int argc, char *argv[])
{
    mat A(2,2,{1,0,0,1});
    mat P1(2,2,{0,1,-1,0});
    mat P2 = -P1;
    mat M1 = P1*A*2;
    mat M2 = P2*A;
    vec y0({0,(-2+sqrt(12))/2}),b({2.,8.}),B1 = P1*b;
    auto f = [A,b] (const vec& x){
        return (x.transpose()*A*x + x.transpose()*b)(0,0);
    };
    vec bp = M1.invert()*B1;
    scalar t = 0,dt = 0.2;
    uint N = 40;
    std::vector<vec> O1(N),O2(N);
    for (uint k = 0;k<N;k++){
        O1[k] = exp(M1*t)*y0 - bp;
        std::cout << f(O1[k]) << std::endl;
        //O2[k] = exp(M2*t)*y0 - P1*A.solve(b);
        t += dt;
    }
    QApplication a(argc,argv);
    Plot_window w; w.resize(500,500);

    Plot_tab* T = w.add_tab("my first tab");
    Plot_frame* F= T->add_frame();
    Plot_layer* L = F->add_layer();

    L->new_2D_curve(O1)->fix_plot_in_rect(0,0,10);
    //L->new_2D_curve(O2)->fix_plot_in_rect(0,0,5);
    w.show();
    return a.exec();
}
