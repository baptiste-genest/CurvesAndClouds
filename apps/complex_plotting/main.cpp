#include <curvesandcloud.h>

using namespace cnc;

complex_scalar old_w1,old_w2;
std::vector<complex_scalar> lattice;

std::vector<complex_scalar> gen_lattice(complex_scalar w1,complex_scalar w2){
    if (w1 == old_w1 && w2 == old_w2)
        return lattice;
    std::cout << "new lattice" << std::endl;
    constexpr static int N = 6;
    std::vector<complex_scalar> R;
    for (int j = -N;j<N;j++)
        for (int i = -N;i<N;i++)
            if (j || i)
            R.push_back(w1*scalar(i) + w2*scalar(j));
    lattice = R;
    old_w1 = w1;
    old_w2 = w2;
    return R;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    range X{-2,2};
    auto a = w.add_mutable_scalar_by_cursor(X,"a");
    auto b = w.add_mutable_scalar_by_cursor(X,"b");
    auto c = w.add_mutable_scalar_by_cursor(X,"c");
    auto d = w.add_mutable_scalar_by_cursor(X,"d");
    //auto n = w.add_mutable_scalar_by_cursor({0,10},"n",10);

    auto f = [a,b,c,d] (complex_scalar z) {
        complex_scalar z1(a,b);
        complex_scalar z2(c,d);
        auto L = gen_lattice(z1,z2);
        complex_scalar sum = 1./(z*z);
        for (const auto& l : L)
            sum += 1./((z+l)*(z+l)) - 1./(l*l);
        /*
        constexpr static uint N = 5;
        complex_scalar Z = std::pow(z-z1,n);
        for (uint k = 0;k<N;k++)
            Z /= z-std::polar(1.,2*M_PI*k/N);
            */
        return sum;
    };

    PlotTab* T = w.add_tab("complex plotting");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();


    //auto CFP = L->new_complex_function_plot(f,X,X);
    auto CFP = L->new_complex_function_plot([] (complex_scalar z) {
            complex_scalar w(1,1);
            return std::conj(z)*w;
},X,X);
    CFP->set_resolution(300);
    CFP->set_dynamic();

    w.show();
    return App.exec();
}
