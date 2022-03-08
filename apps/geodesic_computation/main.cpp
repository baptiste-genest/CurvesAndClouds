#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::symbolic;

using Tensor = std::vector<smat>;

vec change_coordinate_system(const svec& map,const vec& x,const vec& v){
    auto J = map.jacobian()(x);
    return J.solve(v);
}

smat invert_diag_smat(const smat& S){
    uint n = S.getHeight();
    smat iM(n,n);
    for (uint i = 0;i<n;i++)
        iM(i,i) = pow(S(i,i),-1);
    return iM;
}

Tensor getChristoffelSymbols(const smat& g){
    auto ig = invert_diag_smat(g);
    uint N = g.getHeight();
    Tensor gamma(N,smat(N,N));
    for (uint i = 0;i<N;i++)
        for (uint k = 0;k<N;k++)
            for (uint l = 0;l<N;l++){
                for (uint m = 0;m<N;m++)
                    gamma[i](l,k) = gamma[i](l,k) + ig(m,i)*(g(k,m)(l) + g(l,m)(k) - g(l,k)(m))*0.5;
            }
    return gamma;
}

bool isNan(const mat& A){
    for (uint j = 0;j<A.rowNum();j++)
        for (uint i = 0;i<A.colNum();i++)
            if (std::isnan(A(i,j)))
                return true;
    return false;
}

using stop_func = std::function<bool(const vec& x)>;
struct Manifold{
    smat metric_tensor;
    svec map;
    Tensor Gamma;
};


cloud solveGeodesic(vec x,vec v,scalar dt,const Manifold& M,const stop_func& stop,uint itermax = 1000) {
    const auto& g = M.metric_tensor;
    const auto& G = M.Gamma;
    uint N = g.getHeight();
    vec a(N);
    cloud traj;
    std::vector<ValuationPair> V;
    for (uint i = 0;i<x.size();i++)
        V.push_back({i,x(i)});
    for (uint iter = 0;iter<itermax;iter++){
        for (uint i = 0;i<N;i++)
            V[i].second = x(i);
        for (uint k = 0;k<N;k++){
            a(k) = 0;
            auto CS = G[k](x);
            if (isNan(CS))
                return traj;
            a(k) += -v.scalar_product(CS*v);
        }
        v += a*dt;
        x += v*dt;
        traj.add_point(M.map(x));
    }
    return traj;
}

void plot2DSchwartzchild(cnc::PlotLayer* L){
    using namespace linear_utils;
    svec map(2);
    smat g(2,2);
    vec x(2),v(2);
    Variable r,th;
    scalar m = 0.5;
    map(0) = r*cos(th);
    map(1) = r*sin(th);
    auto schwartz = 1.-2*m/(r);
    scalar event_horizon = 2*m;
    g(0,0) = pow(schwartz,-1);
    g(1,1) = r*r;
    Manifold M = {g,map,{}};
    M.Gamma = getChristoffelSymbols(g);

    vec cart_v0 = vec2(1,0);
    scalar dt = 0.02;
    uint N = 5./dt;
    std::cout << N << std::endl;
    auto J = map.jacobian();
    for (int h = -40;h<=40;h+= 4){
        vec polar_x0 = linear_utils::toPolar(-2,h/20.);
        vec polar_v0 = algo::solve22(J(polar_x0),cart_v0);
        auto traj = solveGeodesic(polar_x0,polar_v0,dt,M,[event_horizon](const vec& x){
            return x(0) < event_horizon + 1e-5;
        },N).subsample(0.1);
        L->new_2D_curve(traj);
    }
}

int main(int argc, char *argv[]) {
    using namespace cnc::linear_utils;
    scalar l = 4;
    range R{-l,l};

    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    PlotTab *T = Window.add_tab("my first tab");
    PlotFrame *F = T->add_frame();
    PlotLayer *L = F->add_grid_layer(R,R,false);
    plot2DSchwartzchild(L);

    /*
    cloud traj;
    bool D2 = true;
    scalar rad;
    scalar m = 10;

    if (D2){
        N = 2;
    } else {
        N = 3;
        vec x0 = vec3(20,0.01,M_PI*.25);
        vec cart_v0 = vec3(0,1,0);
        auto schwartz = 1.-2*m/(r);
        smat g(N,N);
        g(0,0) = pow(schwartz,-1);
        g(1,1) = pow(r,2);
        g(2,2) = pow(r*sin(th),2);
        Variable r,th,phi;
        svec map(N);
        map(0) = r*sin(th)*cos(phi);
        map(1) = r*sin(th)*sin(phi);
        map(2) = r*cos(th);
    smat g(3,3);
    g(0,0) = 1.;
    g(1,1) = r*r;
    g(2,2) = r*r*sin(th)*sin(th);
        vec spheric_v0 = change_coordinate_system(map,x0,cart_v0);
        traj = solveGeodesic(x0,spheric_v0,0.01,g,map,30000);
    }
    rad = 2*m;
    std::cout << "RADIUS = " << rad << std::endl;
    if (D2){
        L->new_2D_curve(traj);
    } else {
        auto th = Window.add_mutable_scalar_by_cursor({0,2*M_PI},"th");
        auto E = L->add_euclidean_plane(R,R);
        E->add_object<euclid::Circle>(linear_utils::vec2(),rad);
        traj = traj.subsample(0.1);
        for (const auto& x : traj.points){
            E->add_object<euclid::Point>([th,x](){
                return linear_utils::vec2(algo::geometry::Ry(th)*x);
            });
        }
    }
        */


    Window.show();
    return App.exec();
}
