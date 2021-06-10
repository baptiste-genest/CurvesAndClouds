#include <curvesandcloud.h>

using namespace cnc;
using namespace algo;
#include <iterator>

int main(int argc, char *argv[])
{
    const uint nb_particules = 3,
            DIM = nb_particules*4;
    mat D(DIM,DIM);
    scalar momentum = 0,
            air = 0.,
            g = 15,
            dt = 0.05;
    D(1,0)= -1;
    D(0,1)= 1;
    D(6,4)= 1;
    D(7,5)= 1;
    D(10,8)= 0.9;
    D(11,9)= .9;
    D(0,6)= g;
    D(1,7)= g;
    D(4,6) = -g + momentum;
    D(5,7) = -g + momentum;
    D(6,6) = -air;
    D(7,7) = -air;
    D(4,10)= g;
    D(5,11)= g;
    D(8,10) = -g + momentum;
    D(9,11) = -g + momentum;
    D(10,10) = -air;
    D(11,11) = -air;
    std::cout << D << std::endl;
    uint N = 30/dt;
    mat pulse = exp(D*dt);
    mat state = chaskal::Id<scalar>(DIM);
    vec X0(DIM);
    X0(0) = 1;
    X0(4) = 0;
    X0(8) = 1;
    vec X(DIM);
    std::array<std::vector<vec>,nb_particules> traj;
    for (uint k = 0;k<nb_particules;k++)
        traj[k].resize(N);
    for (uint k = 0;k<N;k++){
        X = state*X0;
        state = pulse*state;
        for (uint i = 0;i<nb_particules;i++)
           traj[i][k] = vec({X(i*4),X(i*4+1)});
    }
    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    algo::calculus::nodes t = algo::calculus::get_lin_space(0,1,(int)N);
    algo::calculus::nodes d01(N),d12(N);
    for (uint k = 0;k<N;k++){
        d01[k] = traj[0][k].distance(traj[1][k]);
        d12[k] = traj[1][k].distance(traj[2][k]);
    }

    PlotTab* T = w.add_tab("High Dimension Linear Differential System");
    PlotFrame* F= T->add_frame();
    PlotGroup* G = F->add_plot_group();
    for (uint k = 0;k<nb_particules;k++)
        G->new_2D_curve(traj[k]);
    PlotGroup* G2 = T->add_frame()->add_plot_group();
    G2->new_function_plot(algo::calculus::build_1D_linear_interpolator(t,d01),{0,1},min_max_range);
    G2->new_function_plot(algo::calculus::build_1D_linear_interpolator(t,d12),{0,1},min_max_range);

    w.show();
    return a.exec();
}
