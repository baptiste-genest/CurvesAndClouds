#include <curvesandcloud.h>

using namespace cnc;

std::vector<scalar> compute_pij(const cloud& C,scalar sigma){
    uint n = C.size(),N = n*n;
    std::vector<scalar> pibarj(N);
    std::vector<scalar> pjbari(N);
    std::vector<scalar> pij(N);
    //scalar l = 1/(2*sigma*sigma);
    for (uint i = 0;i<n;i++){
        for (uint j = 0;j<n;j++){
            pibarj[j*n+i] = exp(C[i].distance2(C[j]));
            pjbari[j*n+i] = exp(C[i].distance2(C[j]));
        }
    }

    return pij;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("t-SNE");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();


    w.show();
    return App.exec();
}
