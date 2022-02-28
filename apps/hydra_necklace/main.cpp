#include <curvesandcloud.h>

using namespace cnc;

namespace comp {
cscalar i(0.,1.);
}

uint MAXITER = 6;

struct rse{
    bool is_inf = false;
    cscalar value;
};

cmat words[4];
struct circle{
    cscalar center;
    scalar radius;
};

namespace moebius {
cscalar act(const cmat& A,const cscalar& z){
    return (A(0,0)*z+A(1,0))/(A(0,1)*z+A(1,1));
}

circle act(const cmat& A,const circle& C){
    auto z = act(A,C.center);
    return {z,std::abs(z-act(A,C.center+C.radius))};
}
}

void plotSchotkyGroup(euclid::EuclideanPlane* E,const circle& C1,const circle& C2,const cmat& A = chaskal::Id<cscalar>(2),int previous=-1,uint n = 0){
    if (n >= MAXITER){
        E->add_object<euclid::Circle>(C1.center,C1.radius);
        E->add_object<euclid::Circle>(C2.center,C2.radius);
        return;
    }
    for (int i = 0;i<4;i++){
        int j = 4-i;
        if (j != previous){
            auto NA = words[i]*A;
            plotSchotkyGroup(E,moebius::act(NA,C1),moebius::act(NA,C2),NA,i,n+1);
        }
    }
}


int main(int argc, char *argv[])
{
    words[0] = cmat(2,2,{1,0,-2.*comp::i,1.});
    words[1] = cmat(2,2,{1.-comp::i,1.,1.,1.+comp::i});
    words[2] = words[1].invert();
    words[3] = words[0].invert();
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();

    scalar l = 1.5;
    range r{-l,l};

    PlotLayer* L = F->add_grid_layer(r,r,false);
    auto E = L->add_euclidean_plane(r,r);
    circle C1{comp::i-0.3,0.3};
    circle C2{-comp::i+0.1,2.6};
    plotSchotkyGroup(E,C1,C2);

    w.show();
    return App.exec();
}
