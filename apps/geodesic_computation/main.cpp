#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::symbolic;

using Tensor = std::vector<smat>;

uint N;

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
    Tensor gamma(N,smat(N,N));
    for (uint i = 0;i<N;i++)
        for (uint k = 0;k<N;k++)
            for (uint l = 0;l<N;l++){
                for (uint m = 0;m<N;m++)
                    gamma[i](l,k) = gamma[i](l,k) + ig(m,i)*(g(k,m)(l) + g(l,m)(k) - g(l,k)(m))*0.5;
            }
    std::cout << "CHRISTOFFEL SYMBOLS" << std::endl;
    for (uint i = 0;i<N;i++)
        std::cout << gamma[i].print() << std::endl;
    return gamma;
}

std::vector<vec> solveGeodesic(vec x,vec v,scalar dt,const smat& g,const svec& map,uint itermax = 1000) {
    auto G = getChristoffelSymbols(g);
    vec a(N);
    std::vector<vec> traj;
    std::vector<ValuationPair> V;
    a.plain_log = true;x.plain_log = true;v.plain_log = true;
    for (uint i = 0;i<x.size();i++)
        V.push_back({i,x(i)});
    for (uint iter = 0;iter<itermax;iter++){
        for (uint i = 0;i<N;i++)
            V[i].second = x(i);
        for (uint k = 0;k<N;k++){
            a(k) = 0;
            auto CS = G[k](x);
            //std::cout << CS << std::endl;
            a(k) += -v.scalar_product(CS*v);
            /*
            for (uint i = 0;i<N;i++)
                for (uint j =0 ;j<N;j++)
                    a(k) += -G[k](i,j)(V).real()*v(i)*v(j);
                    */
        }
        /*
        std::cout << "acc: " << a ;
        std::cout << "vel: " << v ;
        std::cout << "pos: " << x << std::endl;
        */
        v += a*dt;
        x += v*dt;
        traj.push_back(map(x));
    }
    return traj;
}

int main(int argc, char *argv[]) {
    using namespace cnc::linear_utils;
    cloud traj;
    bool D2 = false;
    if (D2){
        N = 2;
        Variable r,th;
        svec map(2);
        map(0) = r*cos(th);
        map(1) = r*sin(th);
        smat g(2,2);
        g(0,0) = 1.;
        g(1,1) = r*r;
        std::cout << g.print() << std::endl;

        vec x = linear_utils::vec2(2.,M_PI*0.25);

        vec x0 = vec2(1.,M_PI*0.25);
        vec cart_v0 = vec2(0,1);
        vec polar_v0 = algo::solve22(map.jacobian()(x0),cart_v0);
        std::cout << polar_v0 << std::endl;

        traj = solveGeodesic(x0,polar_v0,0.001,g,map);
    } else {
        N = 3;
        Variable r,th,phi;
        svec map(N);
        map(0) = r*sin(th)*cos(phi);
        map(1) = r*sin(th)*sin(phi);
        map(2) = r*cos(th);
        vec x0 = vec3(6.1,0.1,0.1);
        vec cart_v0 = vec3(1,1,1);
        scalar m = 3;
        auto schwartz = 1.-2*m/(r);
        smat g(N,N);
        g(0,0) = pow(schwartz,-1);
        g(1,1) = pow(r,2);
        g(2,2) = pow(r*sin(th),2);
        std::cout << "RADIUS = " << schwartz.evaluate({r == x0(0)}) << std::endl;
        /*
        smat g(3,3);
        g(0,0) = 1.;
        g(1,1) = r*r;
        g(2,2) = r*r*sin(th)*sin(th);
        */
        vec spheric_v0 = change_coordinate_system(map,x0,cart_v0);
        traj = solveGeodesic(x0,spheric_v0,0.01,g,map,30000);
    }

    scalar l = 10;
    range R{-l,l};

    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    PlotTab *T = Window.add_tab("my first tab");
    PlotFrame *F = T->add_frame();
    PlotLayer *L = F->add_grid_layer(R,R,false);

    if (D2){
        L->new_point_cloud(traj);
    } else {
        auto th = Window.add_mutable_scalar_by_cursor({0,2*M_PI},"th");
        auto E = L->add_euclidean_plane(R,R);
        traj = traj.subsample(0.1);
        for (const auto& x : traj.points){
            E->add_object<euclid::Point>([th,x](){
                return linear_utils::vec2(algo::geometry::Ry(th)*x);
            });
        }
    }

    //L->new_2D_curve(traj);

    Window.show();
    return App.exec();
}
