#include <curvesandcloud.h>

using namespace cnc;

inline scalar gaussian(scalar x, scalar α, scalar μ, scalar σ1, scalar σ2) {
    scalar t = (x - μ) / (x < μ ? σ1 : σ2);
    return α * exp(-(t * t) / 2);
}

scalar xbar(scalar l){
    return gaussian(l,  1.056, 5998, 379, 310) +
            gaussian(l,  0.362, 4420, 160, 267) +
            gaussian(l, -0.065, 5011, 204, 262);
}
scalar ybar(scalar l){
    return gaussian(l,  0.821, 5688, 469, 405) +
            gaussian(l,  0.286, 5309, 163, 311);
}
scalar zbar(scalar l){
    return gaussian(l,  1.217, 4370, 118, 360) +
            gaussian(l,  0.681, 4590, 260, 138);
}

vec xyzFromWavelength(scalar l) {
    vec color(3);
    color(0) = xbar(l);
    color(1) = ybar(l);
    color(2) = zbar(l);
    return color;
}

vec proj(const vec& x){
    return x.trunc(2)/(x(0)+x(1)+x(2));
}

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();
    int lf = 380,hf = 720;
    scalar nm = 1e-1;
    range vision{lf/nm,hf/nm};

    L->new_function_plot(zbar,vision,min_max_range);
    L->new_function_plot(xbar,vision,min_max_range);
    L->new_function_plot(ybar,vision,min_max_range);

    cloud rgb;
    for (int l = lf;l<=hf;l++){
        vec c = xyzFromWavelength(l/nm);
        rgb.add_point(proj(c));
    }
    L->new_point_cloud(rgb);

    w.show();
    return a.exec();
}
