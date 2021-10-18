#include <curvesandcloud.h>

using namespace cnc;

int N = 20;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("rot cylinder");
    PlotFrame* F= T->add_frame();
    range r{-2,2};
    auto G = F->add_grid_layer(r,r,false);

    auto th = w.add_mutable_scalar_by_cursor({0,M_PI},"theta");

    for (int k = 0;k<N;k++){
        euclid::placer p1 = [k] () {
            float alpha = 2*M_PI*float(k)/N;
            return vec({std::cos(alpha),-1});
        };
        euclid::placer p2 = [k,th] () {
            float alpha = 2*M_PI*float(k)/N;
            float x = std::cos(alpha);
            float y = std::sin(alpha);
            return vec({std::cos(th)*x - std::sin(th)*y,1});
        };
        G->add_line(p1,p2,2);
    }

    w.show();
    return App.exec();
}
