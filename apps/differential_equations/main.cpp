#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::calculus;
using namespace cnc::algo::differential_equations;

vec f(const vec& x,scalar){
    return vec({x(1),-x(0)}).normalize();
}

scalar u0(scalar x) {
    scalar a = 2*M_PI;
    return std::pow(std::sin(a*x) + std::sin(a*3*x)*0.2f,2);
}

scalar z(scalar ){
    return 0.f;//std::pow((x-0.5),2);
}

scalar wave(scalar x){
    return 0.5*std::exp(-40*(x-0.5)*(x-0.5));
    if (x < 0.5f)
        return x;
    return 0.f;
}

scalar_function_2D build_nth_lagrange_modulus(uint n){
    return [n] (scalar x,scalar y) {
        scalar f = 1.f;
        std::complex<scalar> z = 1.f,z0(x,y);
        for (uint k = 1;k<=n;k++){
            f /= k;
            z += std::pow(z0,scalar(k))*f;
        }
        scalar r = std::abs(z);

        if (r > scalar(1.1))
            return scalar(0);

        return r;
    };
}

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    {
        using namespace ODE;
        PlotTab* T = w.add_tab("ODE solving");
        PlotFrame* F= T->add_frame();
        PlotLayer* L = F->add_layer();

        scalar dt = 0.3;
        scalar t0 = 0.f;
        vec x0({1.f,0.f});
        ODE_scheme ee = build_euler_explicit(f,dt);
        ODE_scheme rk2 = build_runge_kutta_2(f,dt);
        ODE_scheme rk4 = build_runge_kutta_4(f,dt);

        auto S1 = extract_space_steps(solve_ODE(rk4,x0,t0,dt,100));
        auto P = L->new_2D_curve(S1);
        auto S2 = extract_space_steps(solve_ODE(rk2,x0,t0,dt,100));
        L->new_2D_curve(S2)->set_range_reference(P);
        auto S3 = extract_space_steps(solve_ODE(ee,x0,t0,dt,100));
        L->new_2D_curve(S3)->set_range_reference(P);
        range x = {-3,3};
        auto F2 = T->add_frame()->add_layer();

        auto CVG = build_nth_lagrange_modulus(4);

        F2->new_level_curve(CVG,x,x,10);
        tex_stream ts;
        ts << "ODE solving: comparing numerical schemes" << tex::endl;
        ts << "Euler Explicit : Order 1" << tex::endl;
        ts << "RK2 : Order 2" << tex::endl;
        ts << "RK4 : Order 4" << tex::endl;
        ts << tex("\\Delta t = 0.3") << tex::endl;
        T->add_frame_at(0,1,2)->add_layer()->add_text_frame_from_tex_stream(ts);
    }
    {
        using namespace PDE;
        using namespace PDE::D1;
        PlotTab* T = w.add_tab("PDE solving schemes");
        scalar dt = 0.0001;
        scalar dx = 0.02;

        scalar xa = 0.f,xb = 1.f;
        range x_range({xa,xb});
        nodes X = get_lin_space(x_range,dx);
        uint n = X.size();
        uint NB_STEPS = 90;

        mat A = build_1D_laplacian_operator(n,dx,dirichlet)*(-1.f);

        vec B = build_initial_solution(X,z);
        vec U0 = build_initial_solution(X,u0);
        U0(0) = 1.f;

        PDE_scheme EE = build_euler_explicit(A,B,dt,dirichlet);
        PlotFrame* FEE = T->add_frame();
        FEE->set_nb_layer_per_second(30);
        PDE_steps EE_steps = solve_PDE(EE,U0,NB_STEPS);
        for (const vec& u : EE_steps){
            FEE->add_layer()->new_function_plot(build_1D_linear_interpolator(X,convert_to_nodes(u)),x_range,same_than_x_range);
        }

        std::cout << "CFL number " << dt/(dx*dx) << std::endl;

        dt = 0.01;

        PDE_scheme EI = build_euler_implicit(A,B,dt,dirichlet);
        PlotFrame* FEI = T->add_frame();
        FEI->set_nb_layer_per_second(30);
        PDE_steps EI_steps = solve_PDE(EI,U0,NB_STEPS);
        for (const vec& u : EI_steps){
            FEI->add_layer()->new_function_plot(build_1D_linear_interpolator(X,convert_to_nodes(u)),x_range,same_than_x_range);
        }

        dt = 0.01;

        PDE_scheme CN = build_crank_nicholson(A,B,dt,dirichlet);
        PlotFrame* FCN = T->add_frame();
        FCN->set_nb_layer_per_second(30);
        PDE_steps CN_steps = solve_PDE(CN,U0,NB_STEPS);
        for (const vec& u : CN_steps){
            FCN->add_layer()->new_function_plot(build_1D_linear_interpolator(X,convert_to_nodes(u)),x_range,same_than_x_range);
        }

        boundary_condition_type bct = periodic;
        mat S = build_1D_advection_operator(n,dx,bct);

        U0 = build_initial_solution(X,wave);
        dt = 0.01;

        PDE_scheme CNA = build_crank_nicholson(S,B,dt,bct);
        PlotFrame* FCNA = T->add_frame();
        FCNA->set_nb_layer_per_second(30);
        PDE_steps CNA_steps = solve_PDE(CNA,U0,NB_STEPS);
        for (const vec& u : CNA_steps){
            auto L = FCNA->add_layer();
            L->new_function_plot(build_1D_linear_interpolator(X,convert_to_nodes(u)),x_range,same_than_x_range);
            L->new_function_plot(wave,x_range,same_than_x_range);
        }
    }

    w.show();
    return a.exec();
}
