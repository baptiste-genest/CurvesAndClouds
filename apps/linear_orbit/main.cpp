#include <curvesandcloud.h>

using namespace cnc;

cscalar orbit_test(cscalar x,cscalar x0,cscalar lambda){
    return std::log(x/x0)/std::log(lambda);
}

mat pow(const mat& M,scalar t){
    return chaskal::Diag<scalar>({std::pow(cscalar(M(0,0)),t).real(),std::pow(cscalar(M(1,1)),t).real()});
}

int main(int argc, char *argv[])
{
    cmat M = chaskal::Diag<cscalar>({cscalar(2.,0),cscalar(3,0)});
    vec x0({1,1});

    algo::calculus::scalar_function_2D f = [M,x0] (scalar x,scalar y) {
        cscalar z1 = orbit_test(cscalar(x),cscalar(x0(0)),M(0,0));
        cscalar z2 = orbit_test(cscalar(y),cscalar(x0(1)),M(1,1));
        return std::abs(z1.real()-z2.real()) + std::abs(z1.imag())+ std::abs(z2.imag());
    };

    /*
    std::vector<vec> traj = {x0};
    for (scalar t = 0;t<3.;t+= 0.1)
        traj.push_back(pow(M,t)*x0);
        */

    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("orbit");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    //L->new_2D_curve(traj);

    range R(1,10);
    L->new_colormap(f,R,R,color_policy::from_zero);

    w.show();
    return a.exec();
}
