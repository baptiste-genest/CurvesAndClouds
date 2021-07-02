#include <curvesandcloud.h>

using namespace cnc;

cscalar moebius(const cmat& M,cscalar z){
    return (M(0,0)*z + M(1,0))/(M(0,1)*z + M(1,1));
}

vec bij(const cscalar& x){
    return vec({x.real(),x.imag()});
}

int main(int argc, char *argv[])
{
    cmat a(2,2),b(2,2),A,B,INV(2,2);
    a(0,0) = 2;a(1,0).imag(-1);
    a(0,1).imag(-1);a(1,1) = 0;
    b(0,0) = 1;b(1,0) = 2;
    b(0,1) = 0;b(1,1) = 1;
    A = a.invert();
    B = b.invert();

    cscalar t = cscalar(1.95859103011179,-.0112785606117658);
    INV(0,0) = t;INV(1,0).imag(-1);
    INV(0,1).imag(-1);INV(1,1) = 0;
    std::cout << INV << std::endl;

    std::array<cmat,4> words = {a,b,A,B};
    cscalar z;
    uint N = 5000;
    cloud klein_group;
    for (uint k = 0;k<N;k++){
        klein_group.add_point(bij(moebius(INV,z)));
        z = moebius(words[rand()%4],z);
    }

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    range R(-2,2);
    PlotLayer* L = F->add_grid_layer(R,R);

    L->new_point_cloud(klein_group,1);

    w.show();
    return App.exec();
}
