#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::symbolic;

using Tensor = std::vector<smat>;

vec change_coordinate_system(const svec& map,const vec& x,const vec& v){
    auto J = map.jacobian()(x);
    return J.solve(v);
}
vec change_coordinate_system(const smat& jacobian,const vec& x,const vec& v){
    auto J = jacobian(x);
    return J.solve(v);
}
vec change_coordinate_system(const mat& J,const vec& v){
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
    for (uint i = 0;i<N;i++){
        for (uint k = 0;k<N;k++)
            for (uint l = 0;l<N;l++){
                for (uint m = 0;m<N;m++)
                    gamma[i](l,k) = gamma[i](l,k) + ig(m,i)*(g(k,m)(l) + g(l,m)(k) - g(l,k)(m));
                gamma[i](l,k) = (gamma[i](l,k)*0.5);
            }
        std::cout << gamma[i].print() << std::endl;
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

void removeNan(mat& A){
    for (uint j = 0;j<A.rowNum();j++)
        for (uint i = 0;i<A.colNum();i++)
            if (std::isnan(A(i,j)))
                A(i,j) = 1.;
}
using stop_func = std::function<bool(const vec& x)>;
struct Manifold{
    smat metric_tensor;
    svec map;
    smat Jacobian;
    Tensor Gamma;
};


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


particule solveGeodesic(particule p,scalar dt,const Manifold& M,const stop_func& stop) {
    const auto& g = M.metric_tensor;
    const auto& G = M.Gamma;
    auto& x = p.p;
    auto& v = p.v;
    uint N = g.getHeight();
    vec a(N);
    while (x(0) < 5.1){
        for (uint k = 0;k<N;k++){
            a(k) = 0;
            auto CS = G[k](x);
            if (isNan(CS))
                std::cout << "non" << std::endl;
            if (stop(x))
                return p;
            a(k) += -v.scalar_product(CS*v);
        }
        if (x(0) < 0)
            throw Cnc_error("error radius coord negative");
        v += a*dt;
        x += v*dt;
    }
    return p;
}

void parralelGeodesic(std::vector<particule>& p,const Manifold& M,uint start,uint end,const stop_func& stop,scalar dt,uint itmax){
    static uint NB_DONE = 0;
    for (uint i = start;i<end;i++){
        p[i].p = linear_utils::toSpherical(p[i].p);
        p[i].v = M.Jacobian(p[i].p).solve(p[i].v);
        p[i] = solveGeodesic(p[i],dt,M,stop);
        NB_DONE++;
        if (NB_DONE % 1000 == 0)
            std::cout << NB_DONE << " done" << std::endl;
    }
}
void parralelPlanarGeodesic(std::vector<particule>& p,const Manifold& M,uint start,uint end,const stop_func& stop,scalar dt){
    using namespace linear_utils;
    static uint NB_DONE = 0;
    particule proj;
    for (uint i = start;i<end;i++){
        proj.index = p[i].index;
        auto x0 = p[i].p.normalize();
        auto x1 = (p[i].p + p[i].v*dt*0.01).normalize();
        x1 = (x1 - x0*x0.scalar_product(x1)).normalize();
        proj.p = vec2(p[i].p.norm(),0);
        proj.v = vec2(x0.scalar_product(p[i].v),x1.scalar_product(p[i].v));
        proj.v = M.Jacobian(proj.p).solve(proj.v);
        auto end = solveGeodesic(proj,dt,M,stop).p;
        p[i].p = toSpherical(x0*end(0)*cos(end(1)) + x1*end(0)*sin(end(1)));

        NB_DONE++;
        if (NB_DONE % 1000 == 0)
            std::cout << NB_DONE << " done" << std::endl;
    }
}

QImage back;

scalar eps = 1e-5;
QColor color_func(const vec& x,scalar radius){
    if (x(0) < radius + eps){
        return QColorConstants::Black;
    }
    scalar th = std::fmod(x(1),M_PI);
    scalar psi = std::fmod(x(2),M_TAU);
    if (psi < 0)
        psi += M_TAU;
    if (th < 0)
        th += M_PI;
    int sx = back.width()*(psi/M_TAU);
    int sy = back.height()*(th/M_PI);
    return back.pixelColor(sx,sy);
    return QColor::fromRgbF(std::pow(std::sin(th),30),0,0).toRgb();
    //return QColor::fromHslF(psi/M_TAU,1.,th/M_PI).toRgb();
    if (linear_utils::SphericalToCartesian(x)(2) < 0)
        return QColorConstants::Yellow;
    return QColorConstants::Blue;
}

void schwartzchildPlane3D(cnc::PlotLayer* L){
    using namespace linear_utils;
    uint dim = 2;
    Variable r,th;
    scalar m = 0.5;
    auto schwartz = (r-2*m)/(r);
    scalar event_horizon = 2*m;

    smat g(dim,dim);
    g(0,0) = pow(schwartz,-1);
    g(1,1) = pow(r,2);

    svec map(dim);
    map(0) = r*cos(th);
    map(1) = r*sin(th);
    int rez = 400;
    std::cout << rez*rez << std::endl;
    auto J = map.jacobian();

    Manifold M = {g,map,J,{}};
    M.Gamma = getChristoffelSymbols(M.metric_tensor);

    frame3D f = {vec3(2,0,0),vec3(0,4,0),vec3(0,0,4)};
    //f = f.transform(algo::geometry::degrees::Ry(20.));
    auto particules = generate_frame(f,10.,rez,rez);

    const scalar dt = 0.1;

    auto fig = L->new_void_figure(rez,rez);
    fig->set_edit_mode();
    const auto stop = [event_horizon](const vec& x){
            return x(0) < event_horizon + eps;
        };
    const uint NB_THREADS = std::thread::hardware_concurrency();
    std::cout << "NB THREADS: " << NB_THREADS << std::endl;
    const uint NB_PIX_PER_THREADS = rez*rez/NB_THREADS;
    std::cout << "number pix per threads: " << NB_PIX_PER_THREADS << std::endl;

    std::vector<std::thread> threads;

    for (uint k = 0;k<NB_THREADS;k++)
        threads.push_back(std::thread(parralelPlanarGeodesic,
                                      std::ref(particules),
                                      std::cref(M),
                                      k*NB_PIX_PER_THREADS,
                                      (k+1)*NB_PIX_PER_THREADS,
                                      stop,
                                      dt));

    for (auto& t : threads)
        t.join();

    for (const auto& p : particules){
        uint i = p.index % rez;
        uint j = p.index / rez;
        auto color = color_func(p.p,event_horizon);
        if (p.index < 2)
            std::cout << p.p << std::endl;
        fig->set_pixel(i,rez-1-j,color);
    }
    fig->set_display_mode();
    fig->save_as_png("../../output/blackhole.png");
}


void Schwartzchild3D(cnc::PlotLayer* L){
    using namespace linear_utils;
    uint dim = 3;
    Variable r,th,phi;
    scalar m = 0.5;
    auto schwartz = (r-2*m)/(r);
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
    int rez = 100;
    std::cout << rez*rez << std::endl;
    auto J = map.jacobian();

    Manifold M = {g,map,J,{}};
    M.Gamma = getChristoffelSymbols(M.metric_tensor);

    frame3D f = {vec3(2,0,0),vec3(0,4,0),vec3(0,0,4)};
    //f = f.transform(algo::geometry::degrees::Ry(90.));
    auto particules = generate_frame(f,10.,rez,rez);

    const scalar dt = 0.04;
    const uint N = 200;

    auto fig = L->new_void_figure(rez,rez);
    fig->set_edit_mode();
    const auto stop = [event_horizon](const vec& x){
            return x(0) < event_horizon + eps;
        };
    const uint NB_THREADS = std::thread::hardware_concurrency();
    std::cout << "NB THREADS: " << NB_THREADS << std::endl;
    const uint NB_PIX_PER_THREADS = rez*rez/NB_THREADS;
    std::cout << "number pix per threads: " << NB_PIX_PER_THREADS << std::endl;

    std::vector<std::thread> threads;

    for (uint k = 0;k<NB_THREADS;k++)
        threads.push_back(std::thread(parralelGeodesic,
                                      std::ref(particules),
                                      std::cref(M),
                                      k*NB_PIX_PER_THREADS,
                                      (k+1)*NB_PIX_PER_THREADS,
                                      stop,
                                      dt,
                                      N));

    for (auto& t : threads)
        t.join();

    for (const auto& p : particules){
        uint i = p.index % rez;
        uint j = p.index / rez;
        auto color = color_func(p.p,event_horizon);
        if (p.index < 2)
            std::cout << p.p << std::endl;
        fig->set_pixel(i,rez-1-j,color);
    }
    fig->set_display_mode();
    fig->save_as_png("../../data/blackhole.png");

}


int main(int argc, char *argv[]) {
    QApplication App(argc, argv);
    PlotWindow Window;
    back = QImage("../../data/background2.png");
    Window.resize(500, 500);

    PlotTab *Tab = Window.add_tab("my first tab");
    PlotFrame *Frame = Tab->add_frame();
    PlotLayer *Layer = Frame->add_layer();

    schwartzchildPlane3D(Layer);

    Window.show();
    return App.exec();
}

