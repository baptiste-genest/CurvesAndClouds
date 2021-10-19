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

    auto th = w.add_mutable_scalar_by_cursor(r2,"theta",100);

    w.get_time_variable(1);

    euclid::placer pl1 = [](){
        return P.get_pos();
    };

    euclid::placer pl2 = [](){
        return vec({0,1});
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
    {
        auto L = T->add_frame()->add_layer();
        L->new_function_plot(f,r2);
        auto E = L->add_euclidean_plane(r2,r2);
        E->add_object<euclid::Point>(pl_graph,4);
    }
    {
        auto L = T->add_frame()->add_layer();
        auto E = L->add_euclidean_plane(r,r);
        auto p1 = E->add_object<euclid::Point>(pl1,4);
        auto p2 = E->add_object<euclid::Point>(pl2,0);
        E->add_object<euclid::Segment>(p1,p2,2);
    }


    w.show();
    return App.exec();
}
