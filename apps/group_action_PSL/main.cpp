#include <curvesandcloud.h>

using namespace cnc;

cmat SU_element(const cscalar& a,const cscalar& b){
    cmat U(2);
    U(0,0) = a;
    U(1,0) = b;
    U(0,1) = std::conj(b);
    U(1,1) = std::conj(a);
    //std::cout << U(0,0)*U(0,0)-U(1,0)*U(1,0) << std::endl;
    return U;
}

vec ctov(const cscalar& z){
    return linear_utils::vec2(z.real(),z.imag());
}


vec action(const cmat& A,const cscalar& z){
    return ctov((A(0,0)*z+A(1,0))/(A(0,1)*z+A(1,1)));
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 1;
    range R{-l,l};
    PlotLayer* L = F->add_grid_layer(R,R,false);

    range vr{-5,5};
    auto br = w.add_mutable_scalar_by_cursor(vr,"re(beta)");
    auto bi = w.add_mutable_scalar_by_cursor(vr,"im(beta)");
    auto E = L->add_euclidean_plane(R,R);

    for (uint k = 1;k<10;k++){
        E->add_object<euclid::Circle>(
                    [br,bi,k](){
            auto beta = cscalar(br,bi);
            cscalar alpha = std::sqrt(1.+beta*beta);
            auto M = algo::fast_pow(SU_element(alpha,beta),k);
            return action(M,cscalar());
        },
        [br,bi,k](){
            auto beta = cscalar(br,bi);
            cscalar alpha = std::sqrt(1.+beta*beta);
            auto M = algo::fast_pow(SU_element(alpha,beta),k);
            auto r = action(M,cscalar()).distance(action(M,cscalar(1.,0.)));
            return r;
        });
    }
    E->add_object<euclid::Circle>(vec(2),1.);
    /*
    E->add_object<euclid::Point>([th,r]() {
    return linear_utils::vec2(r*std::cos(th),r*std::sin(th));
    },3);
    */
    /*
    for (uint k = 0;k<10;k++){
    E->add_object<euclid::Point>([th,r,A]() {
        return action(A,cscalar(r*std::cos(th),r*std::sin(th)));
    },3);
    A = M*A;
    }
    */

    w.show();
    return App.exec();
}
