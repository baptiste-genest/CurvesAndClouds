#include <curvesandcloud.h>

using namespace cnc;

mat make_laplacian_simple(uint n){
    mat L(n,n);
    scalar dx = 1./n;
    scalar idx = 1./dx;
    scalar idx2 = 1.;//idx*idx;
    for (uint k = 1;k<n-1;k++){
        L(k,k) = -2*idx2;
        L(k-1,k) = 1*idx2;
        L(k+1,k) = 1*idx2;
    }
    L(0,0) = 0;
    L(n-1,n-1) = 0;
    return L;
}

mat make_advection_simple(uint n){
    mat L(n,n);
    //scalar dx = 1./n;
    scalar idx = 1.;
    for (uint k = 0;k<n;k++){
        L(k,k) = -idx;
        if (k < n-1)
            L(k+1,k) = idx;
        else
            L(0,k) = idx;
    }
    return L;
}


mat make_laplacian_second(uint n){
    mat L(2*n,2*n);
    scalar dx = 1./n;
    scalar idx = 1./dx;
    scalar idx2 = 1.;//idx*idx;
    for (uint k = 1;k<n-1;k++){
        L(k,k+n) = -2*idx2;
        L(k-1,k+n) = 1*idx2;
        L(k+1,k+n) = 1*idx2;
    }
    for (uint k = 1;k<n-1;k++)
        L(k+n,k) = 1;
    return L;
}

void sample_derivative(vec& U,uint n) {
    scalar dx = 1./n;
    scalar idx = 1.;///dx;
    for (uint k = 0;k<n-1;k++)
        U(k+n) = (U(k+1) - U(k))*idx;
    U(2*n-1) = U(2*n-2);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    scalar dt = 0.5;
    uint N = 50;

    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");

    {
        PlotFrame* F= T->add_frame();
        F->set_nb_layer_per_second(20);
        mat Lp = make_laplacian_simple(N);
        mat pulse = exp(Lp*dt);
        vec U0 = algo::stat::random_var::sample_vector_on_unit_sphere(N,1)[0];
        auto X = algo::calculus::get_lin_space(0,1,(int)N);
        vec U = U0;
        for (uint k = 0;k<20;k++){
            GridLayer* L = F->add_grid_layer({0,1},{-1,1});
            L->new_function_plot(algo::calculus::build_1D_linear_interpolator(X,algo::calculus::convert_to_nodes(U)),{0,1},min_max_range);
            U = pulse*U;
        }
    }

    {
        PlotFrame* F= T->add_frame();
        F->set_nb_layer_per_second(20);
        mat Lp2 = make_laplacian_second(N);
        mat pulse = exp(Lp2*dt);
        vec U0(2*N);
        for (uint k = 0;k<N/2;k++)
            U0(k) = std::sin(M_PI*4.*k/N);
        sample_derivative(U0,N);
        auto X = algo::calculus::get_lin_space(0,1,(int)N);
        vec U = U0;
        for (uint k = 0;k<1000;k++){
            GridLayer* L = F->add_grid_layer({0,1},{-2,2});
            L->new_function_plot(algo::calculus::build_1D_linear_interpolator(X,algo::calculus::convert_to_nodes(U.trunc(N))),{0,1},min_max_range);
            U = pulse*U;
        }
    }
    {
        PlotFrame* F= T->add_frame();
        F->set_nb_layer_per_second(20);
        mat Ap = make_advection_simple(N);
        mat pulse = exp(Ap*dt);
        vec U0 = algo::stat::random_var::sample_vector_on_unit_sphere(N,1)[0];
        auto X = algo::calculus::get_lin_space(0,1,(int)N);
        vec U = U0;
        for (uint k = 0;k<2000;k++){
            GridLayer* L = F->add_grid_layer({0,1},{-1,1});
            L->new_function_plot(algo::calculus::build_1D_linear_interpolator(X,algo::calculus::convert_to_nodes(U)),{0,1},min_max_range);
            U = pulse*U;
        }
    }

    w.show();
    return a.exec();
}
