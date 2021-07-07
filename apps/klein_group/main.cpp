#include <curvesandcloud.h>


using namespace cnc;

cmat normalize(const cmat& M){
    return M*(1./std::sqrt(M.det()));
}

struct circle {
    cscalar center;
    scalar radius;
};

cscalar moebius(const cmat& M,cscalar z){
    return (M(0,0)*z + M(1,0))/(M(0,1)*z + M(1,1));
}

circle moebius(const cmat& M,circle C){
    circle MR;
    cscalar z = C.center - (C.radius*C.radius)/std::conj(M(1,1)/M(0,1) + C.center);
    MR.center = moebius(M,z);
    MR.radius = std::abs(MR.center - moebius(M,C.center + C.radius));
    return MR;
}


vec bij(const cscalar& x){
    return vec({x.real(),x.imag()});
}

void plot(PlotLayer* GL,const circle& C){
    const uint N = 30;
    std::vector<vec> path(N,vec(2));
    for (uint k = 0;k<N;k++){
        scalar th = M_PI*2.*k/(N-1);
        path[k](0) = C.center.real() + std::cos(th);
        path[k](1) = C.center.imag() + std::sin(th);
    }
    GL->new_2D_curve(path);
}

void plot(PlotLayer* L,const std::vector<circle>& C){
    for (const auto& c : C)
        plot(L,c);
}

scalar sec(scalar x){
    return 1./std::cos(x);
}

typedef unsigned short id;
typedef std::vector<id> chain;

cmat generate(const chain& C,const std::array<cmat,4>& words){
    cmat M = chaskal::Id<cscalar>(2);
    for (id i : C)
        M = M*words[i];
    return M;
}

void print(const chain& C){
    return;
    std::array<char,4> names = {'a','b','A','B'};
    for (id i : C)
        std::cout << names[i] << ' ';
    std::cout << std::endl;
}

chain concat(const chain& C,id k){
    chain R = C;
    R.push_back(k);
    return R;
}

void plot_fuschian_group(const std::array<cmat,4>& W,const std::vector<circle>& bases,PlotLayer* L){
    constexpr static std::array<id,4> inverts = {2,3,0,1};
    constexpr static uint nb_levels = 3;
    std::vector<std::vector<chain>> chains(nb_levels);
    for (id k = 0;k<4;k++)
        chains[0].push_back({chain({k})});
    for (id j = 0;j<nb_levels-1;j++){
        for (const auto& chain_in_level : chains[j]){
            for (id k = 0;k<4;k++)
                if (k != inverts[chain_in_level.back()]){
                    chains[j+1].push_back(concat(chain_in_level,k));
                    print(chains[j+1].back());
                }
        }
    }
    for (const auto& level : chains){
        for (const auto& cil : level)
            for (uint k = 0;k<4;k++)
                plot(L,moebius(generate(cil,W),bases[k]));
    }
}

int main(int argc, char *argv[])
{
    scalar th = M_PI/6,c = std::cos(th),s = std::sin(th);
    cmat a(2,2),b(2,2),A,B,INV(2,2);
    a(0,0) = 1;a(1,0).imag(c);
    a(0,1).imag(-c);a(1,1) = 1;
    a *= 1./s;
    b(0,0) = 1;b(1,0) = c;
    b(0,1) = c;b(1,1) = 1;
    b *= 1./s;
    A = a.invert();
    B = b.invert();
    std::array<cmat,4> words = {a,b,A,B};

    std::vector<circle> circles(4);
    for (uint k = 0;k<4;k++){
        circles[k].center = std::pow(cscalar(0,1),k)*sec(th);
        circles[k].radius = std::tan(th);
    }

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    range R(-4,4);
    PlotLayer* L = F->add_grid_layer(R,R);

    plot(L,{cscalar(),1.});
    plot(L,circles);
    plot(L,moebius(b,circles[2]));
    plot(L,moebius(b,circles[1]));
    plot(L,moebius(b,circles[3]));
    //plot_fuschian_group(words,circles,L);

    w.show();
    return App.exec();
}
/*
    cscalar t = cscalar(1.95859103011179,-.0112785606117658);
    INV(0,0) = t;INV(1,0).imag(-1);
    INV(0,1).imag(-1);INV(1,1) = 0;

    std::array<cmat,4> words = {a,b,A,B};
    cscalar z;
    uint N = 5000;
    cloud klein_group;
    for (uint k = 0;k<N;k++){
        klein_group.add_point(bij(moebius(INV,z)));
        z = moebius(words[rand()%4],z);
    }
    */
