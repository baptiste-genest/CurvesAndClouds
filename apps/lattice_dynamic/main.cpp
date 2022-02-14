#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::linear_utils;

int N = 20;

cloud gen_lattice(const vec& w1,const vec& w2){
    cloud L;
    for (int i = -N+1;i<N;i++)
        for (int j = -N+1;j<N;j++)
            if (i || j)
                L.add_point(w1*i+w2*j);
    return L;
}

int main(int argc, char *argv[])
{
    mat A(2,2,{1.,1.,1.,2.});
    auto e = algo::get_2x2_eigenpaires(A);
    auto P = mat22(e[0].vector,e[1].vector);
    auto IP = invert22(P);

    return 0;
            /*
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);
    w.show();
    return App.exec();
            */
}
