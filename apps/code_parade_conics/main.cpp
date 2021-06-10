#include <curvesandcloud.h>

using namespace cnc;

scalar f(scalar x,scalar y){
    return x*x*x + x*x - y*y;
}

scalar df_x(scalar x,scalar){
    return 3*x*x + 2*x;
}

scalar df_y(scalar,scalar y){
    return -2*y;
}

vec df(scalar x,scalar y) {
    return vec({df_x(x,y),df_y(x,y)});
}
int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("Conic interpolation");
    PlotFrame* F= T->add_frame();
    range R(-2,2);
    GridLayer* G = F->add_grid_layer(R,R,true);

    G->new_level_curve_unique(f,R,R);
    G->new_level_curve_unique(df_x,R,R);
    G->new_level_curve_unique(df_y,R,R);
    G->new_vector_field(df,R,R,20);

    w.show();
    return a.exec();
}
