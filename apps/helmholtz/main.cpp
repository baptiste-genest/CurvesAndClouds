#include <curvesandcloud.h>
using namespace cnc;
using namespace cnc::algo::vector_calculus;

cnc::vec z(float x,float y){
    return cnc::vec({std::sin(4*y)+x,y+std::sin(4*x)});
    return cnc::vec({x,y});
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Plot_window w; w.resize(500,500);
    auto T = w.add_tab("Helmholtz decomposition");

    range x = {-1.f,1.f};
    auto X = algo::calculus::get_lin_space(x,10);
    Vector_2D_array V(X,X,z);
    auto helmholtz = V.get_helmoltz_hodge_decomposition();
    const Vector_2D_array& rot = helmholtz.first;
    const Vector_2D_array& div = helmholtz.second;
    auto W = rot+div;

    T->add_frame()->add_layer()->new_vector_field(z,x,x,15);
    T->add_frame()->add_layer()->new_vector_field(rot.get_interpolator(x,x),x,x,15);
    T->add_frame()->add_layer()->new_vector_field(div.get_interpolator(x,x),x,x,15);
    T->add_frame()->add_layer()->new_vector_field(W.get_interpolator(x,x),x,x,15);

    tex_stream to;

    to << "Helmholtz-Hodge decomposition" << tex::endl << tex("V = U + W") << tex::endl << "with" << tex::endl;
    to << tex_system({"\\text{div}(U) = 0","\\text{rot}(W) = 0"}) << tex::endl <<  "where";
    to << tex::endl << tex_system({"U = V - \\nabla A","\\Delta A = \\text{div}(V)"});
    T->add_frame()->add_layer()->add_text_frame_from_tex_stream(to);

    w.show();

    return a.exec();
}
