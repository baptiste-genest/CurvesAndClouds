#include <curvesandcloud.h>

using namespace cnc;

vec lerp(scalar x1,scalar y1,scalar x2,scalar y2){
    scalar a= (y2-y1)/(x2-x1);
    return vec({a,y1-a*x1});
}

cmat FM(int n){
    cmat D(2);
    constexpr static cscalar I(0,1);
    D(0,0) = I/scalar(n);
    D(1,1) = D(0,0);
    D(0,1) = 1.0/(n*n);
    return D;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    tex_stream ts;
    ts << "My first plot!" << tex::endl;

    L->add_text_frame_from_tex_stream(ts);

    w.show();
    return App.exec();
}
