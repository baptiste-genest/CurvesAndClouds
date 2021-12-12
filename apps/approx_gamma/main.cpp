#include <curvesandcloud.h>

using namespace cnc;

scalar gamma(scalar x){
    if (x >= 0 && x <= 1)
        return 0.5*x*(x-1)+1;
    if (x > 1)
        return gamma(x-1)*x;
    return gamma(x+1)/(x+1);
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    range X = {-4.,4.};
    PlotLayer* L = F->add_grid_layer(X,{-3,10});

    L->new_function_plot(gamma,X,given_range);

    w.show();
    return App.exec();
}
