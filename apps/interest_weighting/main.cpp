#include <curvesandcloud.h>

using namespace cnc;

float sum(const std::vector<float>& w){
    float S = 0;
    for (float x : w)
        S += x;
    return S;
}

void normalize(std::vector<float>& w){
    float S = sum(w);
    for (float& x : w)
        x = x/S;
}

cloud apply_power(const std::vector<float>& w,int p){
    cloud W = algo::stat::init_empty_cloud(w.size(),2);
    double S = 0;
    for (uint k = 0;k<w.size();k++){
        W[k](0) = k;
        double x = std::pow(w[k],p);
        W[k](1) = x;
        S += x;
    }
    for (uint k = 0;k<w.size();k++)
        W[k](1) /= S;
    return W;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("interest weights");
    PlotFrame* F= T->add_frame();
    uint N = 5;
    PlotLayer* L = F->add_grid_layer({-1,N+1},{-0.2,1.2},false);

    std::vector<float> W0(N);
    for (float& x : W0)
        x = algo::stat::random_var::random_scalar(0,1);
    normalize(W0);
    std::vector<int> powers = {1,5,10};
    //uint M = 8;
    for (auto p : powers){
        L->new_point_cloud(apply_power(W0,p),4);
    }


    w.show();
    return App.exec();
}
