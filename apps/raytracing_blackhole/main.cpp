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


vec solveGeodesic(vec x,vec v,scalar dt,const Manifold& M,const stop_func& stop,uint itermax = 1000) {
    const auto& g = M.metric_tensor;
    const auto& G = M.Gamma;
    uint N = g.getHeight();
    vec a(N);
    std::vector<ValuationPair> V;
    for (uint i = 0;i<x.size();i++)
        V.push_back({i,x(i)});
    for (uint iter = 0;iter<itermax;iter++){
        for (uint i = 0;i<N;i++)
            V[i].second = x(i);
        for (uint k = 0;k<N;k++){
            a(k) = 0;
            auto CS = G[k](x);
            if (isNan(CS) || stop(x))
                return x;
            a(k) += -v.scalar_product(CS*v);
        }
        if (x(0) < 0)
            throw Cnc_error("error radius coord negative");
        v += a*dt;
        x += v*dt;
    }
    return x;
}

struct particule {
    vec p,v;
    uint index;
};

struct frame3D {
    vec pos,hAxis,vAxis;
    frame3D transform(const mat& A){
        frame3D Fp;
        Fp.pos = A*pos;
        Fp.hAxis = A*hAxis;
        Fp.vAxis = A*vAxis;
        return Fp;
    }
};

std::vector<particule> generate_frame(const frame3D& F,scalar focal_distance,int hrez,int vrez){
    vec cam_pos = F.pos + algo::geometry::cross(F.hAxis,F.vAxis).normalize()*focal_distance;
    std::vector<particule> P(hrez*vrez);
    int hhr = hrez/2;
    int hvr = vrez/2;
    uint index = 0;
    scalar dy = 1./hvr;
    scalar dx = 1./hhr;
    for (int j = -hvr;j<hvr;j++){
        for (int i = -hhr;i<hhr;i++){
            P[index].p = F.pos + F.hAxis*i*dx + F.vAxis*j*dy;
            P[index].v = (P[index].p - cam_pos).normalize();
            P[index].index = index;
            index++;
        }
    }
    return P;
}

scalar eps = 1e-5;
QColor color_func(const vec& x,scalar radius){
    if (x(0) < radius + eps){
        return QColorConstants::Black;
    }
    if (linear_utils::SphericalToCartesian(x)(2) < 0)
        return QColorConstants::Yellow;
    return QColorConstants::Blue;
}

void Schwartzchild3D(cnc::PlotLayer* L){
    using namespace linear_utils;
    uint dim = 3;
    Variable r,th,phi;
    scalar m = 0.5;
    auto schwartz = 1.-2*m/(r);
    scalar event_horizon = 2*m;

    smat g(dim,dim);
    g(0,0) = pow(schwartz,-1);
    //g(0,0) = 1.;
    g(1,1) = pow(r,2);
    g(2,2) = pow(r*sin(th),2);

    svec map(dim);
    map(0) = r*sin(th)*cos(phi);
    map(1) = r*sin(th)*sin(phi);
    map(2) = r*cos(th);
    int rez = 300;

    Manifold M = {g,map,{}};
    M.Gamma = getChristoffelSymbols(M.metric_tensor);

    frame3D f = {vec3(2,0,0),vec3(0,4,0),vec3(0,0,4)};
    f = f.transform(algo::geometry::degrees::Ry(10.));
    auto particules = generate_frame(f,10.,rez,rez);

    const scalar dt = 0.08;
    const uint N = 100;

    auto fig = L->new_void_figure(rez,rez);
    fig->set_edit_mode();

    auto J = map.jacobian();
    for (const auto& p : particules){
        vec spherical_x0 = toSpherical(p.p);
        vec spherical_v0 = J(spherical_x0).solve(p.v);
        auto end_point = solveGeodesic(spherical_x0,spherical_v0,dt,M,[event_horizon](const vec& x){
            return x(0) < event_horizon + eps;
        },N);
        if (p.index % 100 == 0){
            std::cout << p.index << " over " << rez*rez << std::endl;
        }
        //std::cout << end_point(0) << std::endl;
        uint i = p.index % rez;
        uint j = p.index / rez;
        auto color = color_func(end_point,event_horizon);
        if (p.index < 2)
            std::cout << p.p << std::endl;
        fig->set_pixel(i,rez-1-j,color);
    }
    fig->set_display_mode();
}


int main(int argc, char *argv[]) {
  QApplication App(argc, argv);
  PlotWindow Window;
  Window.resize(500, 500);

  PlotTab *Tab = Window.add_tab("my first tab");
  PlotFrame *Frame = Tab->add_frame();
  PlotLayer *Layer = Frame->add_layer();

  /*
  auto Fig = Layer->new_void_figure(255,255);
  Fig->set_edit_mode();
  for (uint j = 0;j<255;j++)
    for (uint i = 0;i<255;i++)
        Fig->set_pixel(i,j,QColor::fromRgb(i,j,0));
        */

  Schwartzchild3D(Layer);


  Window.show();
  return App.exec();
}
