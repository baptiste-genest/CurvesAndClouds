#include <curvesandcloud.h>

using namespace cnc;

float E(float x,float v){
    return 0.5f*v*v - std::cos(x);
}

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    Plot_window w; w.resize(500,500);

    Plot_tab* T = w.add_tab("my first tab");
    Plot_frame* F= T->add_frame();
    Plot_layer* L = F->add_layer();

    L->new_level_curve(E,{-M_PI*3,3*M_PI},{-5.f,5.f},10);

    w.show();
    return a.exec();
}
