#include <curvesandcloud.h>

using namespace cnc;




int main(int argc, char *argv[])
{
    mat A(3,3,
           {4,12,-16,
            12,37,-43,
            -16,-43,98});
    SMB SA(3,3,true);
    SA(0,0) = 4;
    SA(1,0) = 12;
    SA(2,0) = -16;
    SA(1,1) = 37;
    SA(2,1) = -43;
    SA(2,2) = 98;
    mat L = algo::Cholesky(A);
    std::cout << A << std::endl;
    std::cout << L << std::endl;
    std::cout << L*L.transpose() << std::endl;
    SA.print();
    auto ichol = algo::SparsePreconditionners::incompleteCholesky(SA);
    ichol.print();
}


/*
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();


    w.show();
    return App.exec();
    */
