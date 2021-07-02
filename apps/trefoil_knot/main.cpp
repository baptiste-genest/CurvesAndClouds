#include <curvesandcloud.h>

using namespace cnc;
using namespace algo::stat::random_var;

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
    uint sample_size = 3000;
    cloud sample = sample_vector_on_unit_sphere(4,sample_size);
    std::vector<scalar> score(sample_size);
    for (uint k = 0;k<sample_size;k++)
        score[k] = f(sample[k]).norm();
    auto best = std::min_element(score.begin(),score.end());
    x0 = sample[std::distance(score.begin(),best)];
    std::cout << f(x0) << std::endl;
    scalar dt = 1e-3;
    vec X = x0;
    uint N = 30000;
    auto rand = algo::stat::random_var::sample_vector_on_unit_sphere(4,N);
    vec o = rand[0],a;
    mat U,P,OP;
    uint sample_nb = 0;
    cloud knot;
    for (uint k = 0;k<N;k++){
        //o = sample[k%sample_size];
        U = grad_space(X);
        P = U*(U.transpose()*U).invert()*U.transpose();
        a = o-P*o;
        if (a.norm() < 1e-2){
            sample_nb++;
            o = rand[sample_nb];
        }
        X = X + a.normalize()*dt;
        scalar stereo = 1./(1-X(3));
        if (!k)
            knot.add_point(vec({X(0)*stereo,X(1)*stereo}));
        else if (knot.points.back().distance(X) > 1e-2)
            knot.add_point(vec({X(0)*stereo,X(1)*stereo}));
    }
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("trefoil knot");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();
    L->new_point_cloud(knot);


    w.show();
    return App.exec();
}
