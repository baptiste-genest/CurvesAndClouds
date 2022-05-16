#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::linear_utils;

std::vector<vec> sample_ellipse_points(mat A,uint n){
    uint q = 4;
    std::vector<vec> P(q*n);
    scalar th = M_PI/(2*(n-1));
    for (uint i = 0;i<q*n;i++)
        P[i] = A*linear_utils::vec2(cos(i*th),sin(i*th));
    return P;
}

int main(int argc, char *argv[]) {
    mat A(2,2,{2,1,1,5});
    A = A*0.5;
    std::cout << A << std::endl;
    auto Ep = sample_ellipse_points(A,4);

    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    PlotTab *Tab = Window.add_tab("my first tab");
    PlotFrame *Frame = Tab->add_frame();
    scalar x = 4;
    range r{-x,x};
    PlotLayer *Layer = Frame->add_grid_layer(r,r,false);

    auto EE = A.lanczos();

    Layer->new_2D_curve(Ep);
    for (auto x: EE){
        Layer->new_2D_curve({vec2(),x.vector});
    }

    Window.show();
    return App.exec();
}
