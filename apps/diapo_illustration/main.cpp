#include <curvesandcloud.h>

using namespace cnc;


int main(int argc, char *argv[]) {
    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    range X = {0,4*M_PI};
    enum P {
        a,b,c,d
    };
    P lock = c;
    switch (lock) {
    case a:
    {
        PlotTab *Tab = Window.add_tab("chaleur 1");
        PlotFrame *Frame = Tab->add_frame();
        PlotLayer *Layer = Frame->add_grid_layer(X,{-1.5,1.5},false);

        auto t = Window.add_mutable_scalar_by_cursor({0,5},"t");
        auto heat_solution = [t] (scalar x){
            return std::sin(x)*std::exp(-t);
        };
        Layer->new_function_plot(heat_solution,X)->set_dynamic();
    }    break;
    case b:
    {
        PlotTab *Tab = Window.add_tab("échantillion");
        PlotLayer *L1 = Tab->add_frame()->add_grid_layer(X,{-1.5,1.5},false);

        auto f = [] (scalar x){
            return std::sin(x);
        };
        L1->new_function_plot(f,X);

        auto dx = {0.4,0.2,0.1};
        for (const auto& h : dx){
            PlotLayer *L2 = Tab->add_frame()->add_grid_layer(X,{-1.5,1.5},false);
            auto Xe = algo::calculus::get_lin_space(X,h);
            cloud E;
            for (const auto& x : Xe)
                E.add_point(linear_utils::vec2(x,f(x)));
            L2->new_point_cloud(E);
            L2->add_text_frame("dx=" + std::to_string(h) + " ",formula_disposition::upper_right);
        }
    }    break;
    case c:
     {
        PlotTab *Tab = Window.add_tab("discrete diff");
        auto f = [] (scalar x){
            return std::sin(x);
        };
        auto df = [] (scalar x){
            return std::cos(x);
        };
        /*
        PlotLayer *L1 = Tab->add_frame()->add_grid_layer(X,{-1.5,1.5},false);
        L1->new_function_plot(f,X);
        L1->new_function_plot(df,X);
        */
        auto h = 0.2;
        PlotLayer *L2 = Tab->add_frame()->add_grid_layer(X,{-1.5,1.5},false);
            auto Xe = algo::calculus::get_lin_space(X,h);
            cloud E,D,L;
            for (const auto& x : Xe){
                E.add_point(linear_utils::vec2(x,f(x)));
                D.add_point(linear_utils::vec2(x,(f(x+h)-f(x))/h));
                L.add_point(linear_utils::vec2(x,(f(x+h)+f(x-h) - 2*f(x))/(h*h)));
            }
            L2->new_point_cloud(E);
            L2->new_point_cloud(D);
            //L2->new_point_cloud(L);
    }   break;
    case d:
        break;

    }

    Window.show();
    return App.exec();
}
