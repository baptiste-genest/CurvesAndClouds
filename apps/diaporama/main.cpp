#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::sparse;

smat buildEELaplace(uint n,scalar dx,scalar dt){
    SMB L(n);
    scalar CFL = dt/(dx*dx);
    for (uint k = 0;k<n;k++){
        L(k,k) = 1-2.f*CFL;
        if (k > 0)
            L(k-1,k) = CFL;
        if (k < n-1)
            L(k+1,k) = CFL;
    }
    return L;
}

smat buildEILaplace(uint n,scalar dx,scalar dt){
    SMB L(n);
    scalar CFL = -dt/(dx*dx);
    for (uint k = 0;k<n;k++){
        L(k,k) = 1-2.f*CFL;
        if (k > 0)
            L(k-1,k) = CFL;
        if (k < n-1)
            L(k+1,k) = CFL;
    }
    return L;
}


std::vector<vec> vec_to_curve(const vec& u){
    uint N = u.size();
    auto I = algo::calculus::build_range_mapper({0,N},{-1,1});
    std::vector<vec> V(N,vec(2));
    for (uint i = 0;i<N;i++){
        V[i](0) = I(i);
        V[i](1) = u(i);
    }
    return V;
}

int main(int argc, char *argv[]) {
    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    uint N = 100;

    vec T0 = algo::stat::random_var::random_vec(-1,1,N);

    PlotTab *Tab = Window.add_tab("Euler Heat Equation Solver");
    PlotFrame *Frame = Tab->add_frame(4,3);

    uint nb = 500;

    Frame->set_nb_layer_per_second(100);

    scalar dx = 2./N;
    scalar dt = 1e-5;

    auto L = buildEELaplace(N,dx,dt);
    auto L2 = buildEILaplace(N,dx,dt);

    if (true){
        for (uint i = 0;i<nb;i++){
            Frame->add_layer()->new_2D_curve(vec_to_curve(T0));
            T0 = L*T0;
        }
    }
    else {
        for (uint i = 0;i<nb;i++){
            Frame->add_layer()->new_2D_curve(vec_to_curve(T0));
            T0 = L2.conjugate_gradient(vec(N),T0,1e-12);
        }
    }

    Window.show();
    return App.exec();
}
