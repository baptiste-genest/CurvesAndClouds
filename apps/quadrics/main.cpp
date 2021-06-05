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
    for (uint i = 0;i<M.rowNum();i++)
        M(i,i) = 0;
    for (uint j = 0;j<M.rowNum();j++)
        for (uint i = 0;i<j;i++)
            M(i,j) = -M(j,i);
}

vec proj2(const vec& x){
    return vec({x(0),x(1)});
    scalar z = 4+x(2);
    return vec({x(0)/z,x(1)/z})*4;
}

void proj2(std::vector<vec>& T){
    for (vec& p : T)
        p = proj2(p);
}

algo::calculus::scalar_function get_quadratic(const mat& A){
    return [A] (const vec& x){
        return (x.transpose()*A*x)(0,0);
    };
}

std::vector<vec> travel_on_quadric(const vec& x0,const mat& A,scalar dt,uint N){
    mat P = algo::stat::random_var::random_mat(-1,1,3);
    antisymetrize(P);

    mat flow = P*A*2.0;
    mat pulse = exp(flow*dt);
    auto Q = get_quadratic(A);

    scalar_function goal = [Q] (const vec& x) {
        scalar val = Q(x)-1;
        return val*val;
    };

    std::vector<vec> traj(1);
    traj[0] = algo::calculus::optimization::gradient_descent_fixed_step(goal,x0,0.1);
    for (uint k = 1;k<N;k++){
        traj.push_back(pulse*traj.back());
        if (traj.back().norm_inf() > 3)
            return traj;
    }

    return traj;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    mat A = algo::stat::random_var::random_mat(-2,2,3);
    A = (A + A.transpose())*0.5;
    auto RS = get_polar_decomposition(A);
    std::cout << A << std::endl;
    std::cout << RS.second << std::endl;
    auto ee = A.lanczos();
    for (uint k = 0;k<3;k++)
        std::cout << ee[k].value << std::endl;

    const uint N = 80;

    cloud initials = algo::stat::random_var::sample_vector_on_unit_sphere(3,N);


    QApplication App(argc,argv);
    Plot_window w; w.resize(500,500);

    Plot_tab* T = w.add_tab("Quadrics");
    Plot_frame* F= T->add_frame();
    Plot_layer* L = F->add_layer();

    scalar dt = 0.05;
    for (uint k = 0;k<N;k++){
        auto T = travel_on_quadric(initials[k],A,dt,100);
        proj2(T);
        L->new_2D_curve(T)->fix_plot_in_rect(0,0,2);
    }

    w.show();
    return App.exec();
}
