#include <curvesandcloud.h>
#include "sampler.h"
#include "geodesy.h"
#include "knot.h"
#include "fibration.h"

using namespace cnc;

vec proj(const vec& X){
    scalar R = 0.8;
    scalar scale = 2.*R/(R-X(3));
    return X.trunc(2)*scale;
}

scalar arg_diff(complex_scalar z,scalar th){
    complex_scalar nz = z/std::polar(std::abs(z),th);
    return std::abs(nz - 1.);
}

scalar get_z_norm(){
    return cnc::algo::calculus::optimization::newton_method_1D(
                [](scalar x) {return x*x*x + std::abs(knot::sc)*(x*x-1);},
    [](scalar x)  {return 3*x*x + std::abs(knot::sc)*2*x;},1.f,1e-10);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    vec x0(4),s0(4);
    scalar th1 = 0.;
    scalar th2 = (2*th1 + M_PI + ((knot::sc > 0) ? 0. : M_PI))/3;
    scalar zm = get_z_norm();
    scalar wm = std::sqrt(1-zm*zm);
    cscalar z = std::polar(zm,th2);
    cscalar w = std::polar(wm,th1);
    x0 = vec({z.real(),z.imag(),w.real(),w.imag()});
    std::cout << knot::f(x0).norm() << std::endl;
    std::cout << x0.norm() << std::endl;

    scalar dt = 3e-3;
    vec X = x0;
    uint N = 6000;
    sampler S(build_pather(knot::grad_space),dt,N);
    cloud traj = S.sample(x0);
    algo::export_cloud("/home/eulerson/knot.data",traj);

    uint NS = 1e6;
    cloud rs3 = cnc::algo::stat::random_var::sample_vector_on_unit_sphere(4,NS);

    QApplication App(argc,argv);
    PlotWindow Window; Window.resize(500,500);

    PlotTab* T = Window.add_tab("trefoil knot");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();
    auto knot = L->new_point_cloud(traj.apply_to_copy(proj));


    /*
    uint nb_fib = 60;
    N = 3000;
    uint id = 0;

    cloud seibert;
    for (uint i = 0;i<nb_fib;i++){
        bool ok = false;
        for (uint k = id;k<rs3.size();k++){
            if (arg_diff(knot::delta(rs3[k]),fibration::theta()) < 1e-3){
                ok = true;
                s0 = rs3[k];
                id = k+1;
                break;
            }
        }
        if (!ok)
            break;
        sampler Sf(build_pather(fibration::grad_space),dt,N);
        try {
            cloud sf= Sf.sample(s0);
            scalar th_err = 0;
            scalar norm_err = 0;
            for (const auto& p : sf.points){
                scalar ae = arg_diff(knot::delta(p),fibration::theta());
                if (ae > th_err)
                    th_err = ae;
                scalar le = std::abs(1.-p.norm());
                if (le > norm_err)
                    norm_err = le;
            }
            std::cout << "max angle error: " << th_err << " max length error: " << norm_err << std::endl;
            L->new_point_cloud(sf.apply_to_copy(proj))->set_range_reference(knot);
            seibert = seibert + sf;
        }  catch (MatrixException& e) {
            puts("fail");
        }
    }
    algo::export_cloud("/home/eulerson/fibration.data",seibert);
    //return 0;
    */


    Window.show();
    return App.exec();
}
