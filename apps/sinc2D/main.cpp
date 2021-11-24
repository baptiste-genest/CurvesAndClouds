#include <curvesandcloud.h>

using namespace cnc;

scalar sinc(scalar x,scalar y){
    cscalar z(x,y);
    return ((1.+z)/(1.-z)).real();
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("sinc");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    range X = {-1,1};
    L->new_colormap(sinc,X,X,color_policy::from_zero);

    w.show();
    return App.exec();
}
