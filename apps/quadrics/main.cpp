#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::calculus;

mat exp(const mat& M){
    mat X = chaskal::Id<scalar>(3);
    scalar ifac = 1.0;
    mat R = X;
    for (uint k = 1;k<50;k++){
        ifac /= scalar(k);
        X = M*X;
        R = R + X*ifac;
    }
    return R;
}

algo::calculus::vector_function get_grad(const mat& A){
    return [A] (const vec& x){
        return A*x*2.;
    };
}

void antisymetrize(mat& M){
    for (uint j = 0;j<M.rowNum();j++)
        for (uint i = 0;i<j;i++)
            M(i,j) = -M(j,i);
}

vec proj2(const vec& x){
    scalar z = 4+x(2);
    return vec({x(0)/z,x(1)/z})*4;
}

algo::calculus::scalar_function get_quadratic(const mat& A){
    return [A] (const vec& x){
        return (x.transpose()*A*x)(0,0);
    };
}

int main(int argc, char *argv[])
{
    scalar a=1,b=-1,c=1;
    mat A(3,3);
    A(0,0) = a;A(1,1) = b;A(2,2) = c;

    vec y({1.,1.,1.});
    std::cout << y.transpose()*A*y << std::endl;

    mat P(3,3);
    P(1,0) = 1.0;
    P(2,1) = 1.0;
    P(2,0) = 1.0;
    antisymetrize(P);
    std::cout << P << std::endl;
    mat flow = P*A*2.;

    const uint N = 8;
    auto Q = get_quadratic(A);

    scalar_function goal = [Q] (const vec& x) {
        scalar val = Q(x)-1;
        return val*val;
    };

    cloud initials = algo::stat::random_var::sample_vector_on_unit_sphere(3,N);
    for (uint k = 0;k<N;k++){
        initials[k] = algo::calculus::optimization::gradient_descent_fixed_step(goal,initials[k],0.1);
        std::cout << Q(initials[k]) << std::endl;
    }


    std::array<cloud,N> C;


    vec x(3);
    scalar dt = 0.1;
    for (uint i = 0;i<N;i++){
        scalar t=-15*dt;
        for (uint k = 0;k<30;k++){
            x = exp(flow*t)*initials[i];
            C[i].add_point(proj2(x));
            t += dt;
        }
    }


    QApplication App(argc,argv);
    Plot_window w; w.resize(500,500);

    Plot_tab* T = w.add_tab("Quadrics");
    Plot_frame* F= T->add_frame();
    Plot_layer* L = F->add_layer();

    for (uint k = 0;k<N;k++)
        L->new_point_cloud(C[k])->fix_plot_in_rect(0,0,2);

    w.show();
    return App.exec();
}
