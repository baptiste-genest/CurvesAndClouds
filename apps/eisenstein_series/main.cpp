#include <curvesandcloud.h>

using namespace cnc;

cscalar lambert(const cscalar& q,uint a){
    if (std::abs(q) > 1.)
        std::cerr << "won't converge" << std::endl;

    cscalar sum = 0;
    auto qn = q;
    static constexpr uint N = 150;
    for (uint n = 1;n<N;n++){
        sum += std::pow(n,a)*qn/(1.-qn);
        qn = qn*q;
    }
    return sum;
}

cscalar g2(const cscalar& tau){
    static const scalar factor = std::pow(M_PI,4)/45;
    cscalar q = std::exp(cscalar(0,2)*M_PI*tau);
    return factor*(1.+240.*lambert(q,3));
}

cscalar g3(const cscalar& tau){
    static const scalar factor = 2*std::pow(M_PI,6)/945;
    cscalar q = std::exp(cscalar(0,2)*M_PI*tau);
    return factor*(1.-504.*lambert(q,5));
}

cscalar img_by_matrix(const mat& A){
    cscalar z(A(0,0),A(0,1)),w(A(1,0),A(1,1));
    return w/z;
}

int main(int argc, char *argv[])
{
    mat U(2,2,{1,1,0,1.});
    mat V(2,2,{1,0,1,1.});
    auto A = U*V;
    std::cout << A << std::endl;
    auto z = img_by_matrix(A);
    std::cout << g2(z) << ' ' << g3(z) << std::endl;
    /*
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
    */
}
