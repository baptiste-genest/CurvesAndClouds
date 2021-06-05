#include <curvesandcloud.h>

using namespace cnc;

scalar cubic(const mat& A,const vec& X){
    scalar x = X(0),y = X(1);
    return A(0,0)*x*x*x + A(0,1)*x*x*y + A(1,0)*x*y*y + A(1,1)*y*y*y;
}

vec f(scalar t) {
    return vec({std::cos(t),std::sin(t)});
}

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    mat A(2,2,{1,2,1,2});


    PlotTab* T = w.add_tab("Cubics");
    PlotFrame* F= T->add_frame();
    F->set_nb_layer_per_second(10);
    PlotLayer* L = F->add_layer();
    range r = {-2,2};
    L->new_level_curve_unique([A](scalar x,scalar y) {
        return cubic(A,vec({x,y}));
    },r,r,1);

    w.show();
    return a.exec();
}
