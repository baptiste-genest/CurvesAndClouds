#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::dual;
/*

class Geodesic {

    Geodesic(const dualFunc& f) : map(f) {
        params(dim);
    }

    vec position(){
        dualVec X(dim);
        for (uint i = 0;i<dim;i++)
            X(i).setValue(params(i));
        return value(map(X));
    }

    std::vector<dualVec> tangentVectors(){
        std::vector<dualVec> T(dim);
        dualVec X(dim);
        for (uint j = 0;j<dim;j++){
            withRespectTo(X,j);
            T[j] = map(X);
        }
    }

    dualMat metric_tensor(){
        dualMat g(dim);

    }

private:
    const static uint dim = 2;
    vec params;
    dualFunc map;
};

dualVec sphere(const dualVec& X){
    return dualVec({
                   sin(X(0))*cos(X(1)),
                   sin(X(0))*sin(X(1)),
                   cos(X(0))
               });
}
*/

int main()
{
    DualNumber x(2);
    DualNumber y(2);
    x(0) = 4;
    y(0) = 3;
    x(1) = 1;
    y(1) = 0;
    y(2) = 0;
    x(2) = 0;

    DualNumber x2 = x*x;
    DualNumber z = x2*y;

    std::cout << z(0) << ' ' << z(1) << ' ' << z(2) << std::endl;

    return 0;
}
/*
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    tex_stream ts;
    ts << "My first plot!" << tex::endl;

    L->add_text_frame_from_tex_stream(ts);

    w.show();
    return App.exec();

    */
