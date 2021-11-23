#include <curvesandcloud.h>

using namespace cnc;

namespace square {
scalar length = 3;
scalar area = length*length;
scalar half_area = area/2;
}
namespace triangle {
vec center = vec({square::length/3.f,square::length/3.f});
}

namespace data {
uint n_samples = 1000;
}

bool belong_to_centered_triangle(const vec& x){
    if (x(0) < 0 || x(1) < 0)
        return false;
    return x(0)+x(1) < square::length;
}

using shape_tester = std::function<bool(const vec&)>;

scalar compute_shape_isotropy(scalar s,const shape_tester& T){
    static vec x1({1.f,0.f});
    static vec x2({0.f,1.f});
    auto S = algo::stat::random_var::sample_gaussian_vector_by_PC({x1,x2},{s,s},vec(2),data::n_samples);
    uint inside = 0;
    for (auto& p : S.points)
        inside += T(p);
    return scalar(inside)/data::n_samples;
}

algo::calculus::property_2D to_prop(const shape_tester& s){
    return [s] (scalar x,scalar y) {
        return s(vec({x,y}));
    };
}

algo::calculus::scalar_function_1D curryfy(const shape_tester& s){
    return [s] (scalar x){
        return compute_shape_isotropy(x,s);
    };
}

int main(int argc, char *argv[])
{
    auto belong_rect = [] (const vec& x) {
        static scalar ratio = 1;
        return std::abs(x(0)) < square::length*ratio/2 && std::abs(x(1)) < square::length*(1/ratio)/4 ;
    };
    auto belong_tri = [] (const vec& x) {
        return belong_to_centered_triangle(x+triangle::center);
    };
    auto belong_circle = [] (const vec& x) {
        static scalar radius2 = square::half_area/M_PI;
        return x.norm2() < radius2;
    };
    auto belong_square = [] (const vec& x) {
        scalar L = square::length*(sqrt(2)/4);
        return x.norm_inf() < L;
    };

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    range sigma_range = {0,5};

    range x_range = {-square::length*1.5,square::length*1.5};

    auto T = w.add_tab("isotropy");
    {
        auto L = T->add_frame()->add_grid_layer(sigma_range,{0,1.5},false);
        L->new_function_plot(curryfy(belong_rect),sigma_range);
        L->new_function_plot(curryfy(belong_tri),sigma_range);
        L->new_function_plot(curryfy(belong_square),sigma_range);
        L->new_function_plot(curryfy(belong_circle),sigma_range);
    }
    {
        auto L = T->add_frame()->add_layer();
        L->new_boolean_field(to_prop(belong_rect),x_range,x_range,CNC_COLORS[0]);
        L->new_boolean_field(to_prop(belong_tri),x_range,x_range,CNC_COLORS[1]);
        L->new_boolean_field(to_prop(belong_square),x_range,x_range,CNC_COLORS[2]);
        L->new_boolean_field(to_prop(belong_circle),x_range,x_range,CNC_COLORS[3]);
    }

    w.show();
    return App.exec();
}
