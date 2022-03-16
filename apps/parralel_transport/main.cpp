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
        iM(i,i) = pow(S(i,i),-1).simplify();
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
                    gamma[i](l,k) = gamma[i](l,k) + ig(m,i)*(g(k,m)(l) + g(l,m)(k) - g(l,k)(m));
                gamma[i](l,k) = (gamma[i](l,k)*0.5).simplify();
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

cloud solveGeodesic(vec x,vec v,scalar dt,const Manifold& M,uint itermax = 1000) {
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


int main(int argc, char *argv[]) {

    Variable th,phi;
    Manifold S2;
    S2.map = svec(3);
    S2.map(0) = sin(th)*cos(phi);
    S2.map(1) = sin(th)*sin(phi);
    S2.map(2) = cos(th);
    auto J = S2.map.jacobian();

    S2.metric_tensor = J.transpose()*J;
    for (uint i = 0;i<2;i++)
        for (uint j = 0;j<2;j++)
            S2.metric_tensor(i,j) = S2.metric_tensor(i,j).simplify();
    std::cout << S2.metric_tensor.print() << std::endl;
    S2.Gamma = getChristoffelSymbols(S2.metric_tensor);
    for (uint j = 0;j<2;j++)
        std::cout << S2.Gamma[j].print() << std::endl;
    return 0;
    using namespace linear_utils;
    auto G = solveGeodesic(vec2(0.1,0.2),vec2(1,0.1),0.01,S2,500);

    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    PlotTab *Tab = Window.add_tab("my first tab");
    PlotFrame *Frame = Tab->add_frame();
    PlotLayer *Layer = Frame->add_layer();

    tex_stream ts;

    Layer->new_point_cloud(G);

    Window.show();
    return App.exec();
}
