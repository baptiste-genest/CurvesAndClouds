#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::geometry;


cscalar delta(const vec& x){
    cscalar g2(x(0),x(1));
    cscalar g3(x(2),x(3));
    return g2*g2*g2 - g3*g3;
}

cscalar delta_norm(const vec& x){
    auto D = delta(x);
    return D/std::abs(D);
}

vec dx(uint i){
    vec x(4);
    x(i) = 1;
    return x;
}

scalar dt = 1e-3,ds = dt;
std::vector<vec> grad(const vec& x){
    std::vector<vec> G(3,vec(4));
    for (uint i = 0;i<4;i++){
        auto d = (delta_norm(x+dx(i)*dt)-delta_norm(x))/dt;
        G[0](i) = d.real();
        G[1](i) = d.imag();
    }
    G[0].normalize();
    G[1].normalize();
    G[2] = x.normalize();
    return G;
}

vec stereo_proj(const vec& x){
    scalar f = 1./(1-x(3));
    return linear_utils::vec3(x(0),x(1),x(2))*f;
}

int main(int argc, char *argv[])
{
    dt = 1e-5;
    ds = 1e-5;
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);
    PlotTab* T = w.add_tab("seifert");
    auto th = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta");
    scalar l = 2;
    range r{-l,l};
    auto L = T->add_frame()->add_layer();
    auto E = L->add_euclidean_plane(r,r);

    /*
    auto z = std::polar(1.,6.);
    auto g2 = std::pow(z,1./3.);
    auto x = vec({g2.real(),g2.imag(),0.,0.});
    */
    for (uint k = 0;k<50;k++){
        auto x = algo::stat::random_var::sample_vector_on_unit_sphere(4,1)[0];
        auto z = delta_norm(x);
        cloud fibration;
        for (uint i = 0;i<100000;i++){
            fibration.add_point(x);
            auto G = grad(x);
            vec z;
            if (i == 0)
                z = algo::stat::random_var::sample_vector_on_unit_sphere(4,1)[0];
            else
                z = fibration.points[i] - fibration.points[i-1];
            G.push_back(z.normalize());
            G = chaskal::gram_schmidt(G);
            x += G.back()*ds;
            x = x.normalize();
        }
        if (std::abs(delta_norm(x)-z) > 1e-3)
            continue;
        std::cout << k << std::endl;
        //std::cout << delta_norm(x) << ' '  << z << ' ' << x.norm() << std::endl;

        scalar reps = 3e-2;
        vec old = fibration[0];
        for (uint i = 0;i<fibration.size();i++)
            if (old.distance(fibration[i]) > reps){
                const auto& p = fibration[i];
                E->add_object<euclid::Point>([p,th](){
                    return linear_utils::vec2(algo::geometry::Rx(th)*stereo_proj(p));
                });
                old = fibration[i];
            }
    }
    w.show();
    return App.exec();
}
