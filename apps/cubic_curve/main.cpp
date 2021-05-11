#include "curvesandcloud.h"
using namespace cnc;
using namespace cnc::algo::calculus;

scalar_function_2D cubic(const cloud& c){
    mat A(9,9);vec one(9);
    for (uint j = 0;j<9;j++){
        one(j) = 1.f;
        float x = c[j](0);
        float y = c[j](1);
        A(0,j) = x*x*x;
        A(1,j) = y*x*x;
        A(2,j) = y*y*x;
        A(3,j) = y*y*y;
        A(4,j) = x*x;
        A(5,j) = x*y;
        A(6,j) = y*y;
        A(7,j) = x;
        A(8,j) = y;
    }
    /*
    for (uint j = 1;j<=3;j++)
        for (uint i = 0;i<=j;i++)
            std::cout << j-i << ' ' << i << std::endl;
            */
    auto X = A.solve(one);
    return [X] (float x,float y){
        vec A(9);
        A(0) = x*x*x;
        A(1) = y*x*x;
        A(2) = y*y*x;
        A(3) = y*y*y;
        A(4) = x*x;
        A(5) = x*y;
        A(6) = y*y;
        A(7) = x;
        A(8) = y;
        return A.scalar_product(X);
    };
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plot_window w;w.resize(500,500);
    auto T = w.add_tab("Cubic planar curve");
    auto F = T->add_frame();
    for (uint k = 0;k<10;k++){
        cloud C = algo::stat::random_var::sample_uniform_in_square(2,0.8f,9);
        auto f = cubic(C);
        auto L = F->add_layer();
        L->new_level_curve_unique(f,{-1.f,1.f},{-1.f,1.f},1.f);
        L->new_point_cloud(C,4)->fix_plot_in_rect(0,0,1.f);
    }

    w.show();
    return a.exec();
}
