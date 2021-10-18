#include <curvesandcloud.h>

using namespace cnc;

struct pendulum {
    scalar th = 0;
    scalar th_prime = 0;

    scalar dt = 0.01;

    void set_angle(scalar _th){
        th = _th;
        th_prime = 0;
    }

    void integrate() {
        th_prime -= th*dt;
        th_prime *= 0.999;
        th += th_prime*dt;
    }

    vec get_pos() {
        return vec({std::sin(th),1-std::cos(th)});
    }
};

pendulum P;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    range r{-2.5,2.5};
    range r2{-4,4};

    PlotTab* T = w.add_tab("Pendule Simple");
    GridLayer* G1 = T->add_frame()->add_grid_layer(r2,r2,false);
    GridLayer* G2 = T->add_frame()->add_grid_layer(r,r,false);

    auto th = w.add_mutable_scalar_by_cursor(r2,"theta",100);

    w.get_time_variable(1);

    euclid::placer pl1 = [](){
        return P.get_pos();
    };
    euclid::placer plA = [](){
        return vec({0,1});
    };
    euclid::placer plB = [pl1](){
        return pl1();
    };

    auto f = [](scalar x){
        return 1-std::cos(x);
    };

    euclid::placer pl_graph = [f,th] () {
        if (th.time_since_last_update_in_milliseconds() < 1000)
            P.set_angle(th);
        else
            P.integrate();
        return vec({P.th,f(P.th)});
    };

    G1->new_function_plot(f,r2);
    G1->add_point(pl_graph,4);
    G2->add_point(pl1,4);
    G2->add_line(plA,plB,2);


    w.show();
    return App.exec();
}
