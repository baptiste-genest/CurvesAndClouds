#include <curvesandcloud.h>

using namespace cnc;
using namespace euclid;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    range R{-2,2};
    auto E = F->add_layer()->add_euclidean_plane(R,R);

    auto p = w.add_mutable_scalar_by_cursor({0,1},"p");
    //auto n = w.add_mutable_scalar_by_cursor({0,10},"n",10);

    uint N = 3;
    cvec s(N);
    for (uint k = 0;k<N;k++)
        s(k) = std::polar(1.,2.0*M_PI*k/N);
    for (uint power = 0;power < 16;power++){
        placer placers[N];
        Point* points[N];
        for (uint k = 0;k<N;k++){
            placers[k] = [s,k,p,N,power] (){
                auto S = s;
                cmat P(N,N);
                for (uint i = 0;i<N;i++){
                    P(i,i) = p;
                    P((i+1)%N,i) = 1-p;
                }
                for (uint i = 0;i<power;i++)
                    S = P*S;
                return vec({S(k).real(),S(k).imag()});
            };
            points[k] = E->add_object<Point>(placers[k]);
        }
        for (uint k = 0;k<N;k++)
            E->add_object<Segment>(points[k],points[(k+1)%N]);
    }


    w.show();
    return App.exec();
}
