#include <curvesandcloud.h>

using namespace cnc;
using namespace algo;
using namespace chaskal;

int main(int argc, char *argv[])
{
    scalar dt = 0.1;
    complex_scalar idt(0,dt);
    uint N = 300;

    mat A(2,2,{1.1,-1,-1,1});
    mat G = A*(2.);
    cmat S = sqrt(G);
    cmat IS = S.invert();

    cvec y0({complex_scalar(1,0),complex_scalar(0)});
    cvec v0({complex_scalar(2),complex_scalar(-5)});
    cvec V = IS*v0;

    std::vector<vec> traj(N);


    cmat pulsep = algo::exp(S*idt),pulsen = algo::exp(S*(-idt));
    cmat p = Id<complex_scalar>(2),n = p;
    complex_scalar half_real(0.5,0);
    complex_scalar half_im(0,-0.5);
    for (uint k = 0;k<N;k++){
        p = pulsep*p;
        n = pulsen*n;
        traj[k] = Re((p+n)*half_real*y0 + (p-n)*half_im*V);
    }
    range ra = {-5,5};

    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("conics physics");
    PlotFrame* F= T->add_frame();
    GridLayer* grid = F->add_grid_layer(ra,ra);
    grid->new_2D_curve(traj,true);
    grid->new_vector_field([G] (scalar x,scalar y) {return -G*vec({x,y});},ra,ra,20);


    w.show();
    return a.exec();
}
