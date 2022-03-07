#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::symbolic;

using Tensor = std::vector<smat>;

uint N = 2;

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
    return gamma;
}

std::vector<vec> solveGeodesic(vec x,vec v,scalar dt,const smat& g,const svec& map) {
    auto G = getChristoffelSymbols(g);
    vec a(N);
    std::vector<vec> traj;
    std::vector<ValuationPair> V({{0,x(0)},{1,x(1)}});
    for (uint iter = 0;iter<1000;iter++){
        for (uint i = 0;i<N;i++)
            V[i].second = x(i);
        for (uint k = 0;k<N;k++){
            a(k) = 0;
            for (uint i = 0;i<N;i++)
                for (uint j =0 ;j<N;j++)
                    a(k) += -G[k](i,j)(V).real()*v(i)*v(j);
        }
        v += a*dt;
        x += v*dt;
        traj.push_back(map(x));
    }
    return traj;
}

int main(int argc, char *argv[]) {
    using namespace cnc::linear_utils;
    std::vector<vec> traj;
    if (false){
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
    }
    else {
        Variable th,phi;
        svec map(3);
        map(0) = cos(th)*cos(phi);
        map(1) = cos(th)*sin(phi);
        map(2) = sin(th);
        smat g(2,2);
        g(0,0) = 1;
        g(1,1) = sin(th)*sin(th);
        vec x0 = vec2(M_PI*0.1,M_PI*0.5);
        vec cart_v0 = vec3(1,1,0);
        auto J = map.jacobian();
        auto J0 = J(x0);
        std::cout << J.print() << std::endl;
        std::cout << J0 << std::endl;
        vec spheric_v0 = algo::solve22(J0.transpose()*J0,J0.transpose()*cart_v0);
        traj = solveGeodesic(x0,spheric_v0,0.01,g,map);
        for (auto& x : traj)
            x = vec2(x);
    }

    scalar l = 1.5;
    range R{-l,l};

    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    PlotTab *T = Window.add_tab("my first tab");
    PlotFrame *F = T->add_frame();
    PlotLayer *L = F->add_grid_layer(R,R,false);

    L->new_2D_curve(traj);

    Window.show();
    return App.exec();
}
