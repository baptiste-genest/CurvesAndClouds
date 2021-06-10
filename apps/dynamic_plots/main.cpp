#include <curvesandcloud.h>
#include <chrono>

using namespace cnc;

algo::calculus::scalar_function_1D get_f(MutScalar X,MutScalar Y){
    return [X,Y] (scalar x){
        return std::sin(x + Y)*X;
    };
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);
    range r({-5,5});
    MutScalar a = w.add_mutable_scalar_by_cursor(r,"a");
    MutScalar b = w.add_mutable_scalar_by_cursor(r,"b");
    MutScalar c = w.add_mutable_scalar_by_cursor(r,"c");
    MutScalar d = w.add_mutable_scalar_by_cursor(r,"d");

    PlotTab* T = w.add_tab("my first tab");


    T->add_frame()->add_layer()->new_vector_field([a,b,c,d] (scalar x,scalar y) {
        return vec({a*x + b*y,c*x+d*y});
    },r,r,20)->set_dynamic();
    T->add_frame()->add_layer()->new_vector_field([a,b,c,d] (scalar x,scalar y) {
        return vec({a*x + c*y,b*x+d*y});
    },r,r,20)->set_dynamic();

    T->add_frame()->add_layer()->new_vector_field([a,b,c,d] (scalar x,scalar y) {
        mat U(2,2,{a,b,c,d});
        vec X({x,y});
        return (U*U.transpose())*X;
    },r,r,20)->set_dynamic();

    w.show();
    return App.exec();
}
