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

cloud solveGeodesic(vec x,vec v,scalar dt,const smat& g,const svec& map,uint itermax = 1000) {
    auto G = getChristoffelSymbols(g);
    uint N = g.getHeight();
    vec a(N);
    cloud traj;
    traj.add_point(x);
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
        traj.add_point(map(x));
    }
    return traj;
}

struct particule {
    vec p,v;
    uint index;
};

std::vector<particule> generate_frame(const vec& pos,const vec& horizontal,const vec& vertical,scalar focal_distance,int hrez,int vrez){
    vec cam_pos = pos + algo::geometry::cross(horizontal,vertical).normalize()*focal_distance;
    std::vector<particule> P(hrez*vrez);
    int hhr = hrez/2;
    int hvr = vrez/2;
    uint index = 0;
    for (int j = -hvr;j<hvr;j++){
        scalar dy = scalar(j)/hvr;
        for (int i = -hhr;i<hhr;i++){
            scalar dx = scalar(i)/hhr;
            P[index].p = horizontal*i*dx + vertical*j*dy;
            P[index].v = (P[index].p - cam_pos).normalize();
            P[index].index = index;
            index++;
        }
    }
    return P;
}

QColor color_func(const vec& x,scalar radius){
    if (x(0) < radius + 1e-3){
        return QColorConstants::Black;
    }
    return QColorConstants::White;
}

void Schwartzchild3D(cnc::PlotLayer* L){
    using namespace linear_utils;
    uint dim = 3;
    Variable r,th,phi;
    scalar m = 0.5;
    auto schwartz = 1.-2*m/(r);
    scalar event_horizon = 0.5*m;

    smat g(dim,dim);
    g(0,0) = pow(schwartz,-1);
    g(1,1) = pow(r,2);
    g(2,2) = pow(r*sin(th),2);

    svec map(dim);
    map(0) = r*sin(th)*cos(phi);
    map(1) = r*sin(th)*sin(phi);
    map(2) = r*cos(th);
    int rez = 10;

    auto particules = generate_frame(vec3(2,0,0),vec3(0,1,0),vec3(0,0,1),3.,rez,rez);

    const scalar dt = 0.02;
    const uint N = 10000;

    auto fig = L->new_void_figure(rez,rez);
    fig->set_edit_mode();

    auto J = map.jacobian();
    for (const auto& p : particules){
        if (p.index % 10 == 0)
            std::cout << p.index << " over " << rez*rez << std::endl;
        vec spherical_x0 = toSpherical(p.p);
        vec spherical_v0 = J(spherical_x0).solve(p.v);
        auto end_point = solveGeodesic(spherical_x0,spherical_v0,dt,g,map,N).points.back();
        std::cout << end_point(0) << std::endl;
        uint i = p.index % rez;
        uint j = p.index / rez;
        fig->set_pixel(i,j,color_func(end_point,event_horizon));
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

  Schwartzchild3D(Layer);


  Window.show();
  return App.exec();
}
