#include <curvesandcloud.h>

using namespace cnc;

vec f(const vec& x){//w3 + z2
    const scalar& a = x(0);
    const scalar& b = x(1);
    const scalar& c = x(2);
    const scalar& d = x(3);
    return vec({
                a*a*a - 3*a*b + c*c - d*d,
                3*a*a*b - b*b*b + 2*c*d
               });
}

vec grad1(const vec& x){
    const scalar& a = x(0);
    const scalar& b = x(1);
    const scalar& c = x(2);
    const scalar& d = x(3);
    return vec({
               3*a*a - 3*b,
               -3*a,
               2*c,
               -2*d
           });
}

vec grad2(const vec& x){
    const scalar& a = x(0);
    const scalar& b = x(1);
    const scalar& c = x(2);
    const scalar& d = x(3);
    return vec({
               6*a*b,
               3*a*a - 3*b*b,
               2*d,
               2*c
           });
}

scalar S3(const vec& x){
    return x.norm() - 1;
}

vec stereoproj(const vec& x){
    return x.trunc(3)/(2+x(3));
}

mat grad_space(const vec& x){
    std::vector<vec> X(3);
    X[0] = grad1(x);
    X[1] = grad2(x);
    X[2] = x;
    return chaskal::build_basis(X);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    vec x0(4);

    do {
        x0 = cnc::algo::stat::random_var::sample_vector_on_unit_sphere(4,1)[0];
    } while(f(x0).norm2() > 1e-3);
    mat U = grad_space(x0);
    mat P = U*(U.transpose()*U).invert()*U.transpose();
    mat OP = chaskal::Id<scalar>(4) - P;
    scalar dt = 1e-2;
    vec X = x0;
    uint N = 30;
    std::vector<vec> traj(N,vec(2));
    vec o = algo::stat::random_var::sample_vector_on_unit_sphere(4,1)[0];
    for (uint k = 0;k<N;k++){
        vec a = OP*o;
        X = X + a.normalize()*dt;
        traj[k](0) = X(0);
        traj[k](1) = X(1);
    }
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("trefoil knot");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();
    L->new_2D_curve(traj);


    w.show();
    return App.exec();
}
