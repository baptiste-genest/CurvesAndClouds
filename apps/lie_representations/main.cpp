#include <curvesandcloud.h>

using namespace cnc;

mat bracket(const mat& A,const mat& B){
    return A*B - B*A;
}

//-x*del_y
mat opY(int N){
    mat Y(N);
    for (int j = 0;j<N;j++)
        if (j < N-1)
            Y(j+1,j) = -(j+1);
    return Y;
}
mat opX(int N){
    mat X(N);
    for (int j = 0;j<N;j++)
        if (j < N-1)
            X(j,j+1) = -(N-j-1);
    return X;
}
mat opH(int N){
    mat H(N);
    for (int j = 0;j<N;j++)
        H(j,j) = N-1-2*j;
    return H;
}


int main(int argc, char *argv[])
{
    using namespace std;
    if (true)
    {//sl(2,C) rep
        cout << opY(2) << endl;
        cout << opX(2) << endl;
        cout << opH(3) << endl;
        uint n = 2;
        mat H(n);
        H(0,0) = 1;
        H(1,1) = -1;

        vec v(n);
        v(0) = 1;

        mat X(n);
        X(1,0) = 1;
        //X(1,2) = -1;

        mat Y(n);
        Y(0,1) = 1;
        //Y(2,1) = -2;

        cout << v << std::endl;
        vec x = Y*v;
        cout << x << endl;
        cout << H*x << endl;

    }
    else
    {//sl(3,C) rep
        mat H[2];
        for (uint i = 0;i<2;i++){
            H[i] = mat(3);
            H[i](i,i) = 1;
            H[i](i+1,i+1) = -1;
        }

        mat X[3];
        for (uint i = 0;i<3;i++)
            X[i] = mat(3);
        X[0](1,0) = 1;
        X[1](2,0) = 1;
        X[2](2,1) = 1;

        mat Y[3];
        for (uint i = 0;i<3;i++)
            Y[i] = X[i].transpose();

        mat h = H[0];

        std::cout << h << std::endl;
        for (uint i = 0;i<3;i++)
            std::cout << bracket(h,X[i]) << std::endl;
        for (uint i = 0;i<3;i++)
            std::cout << bracket(h,Y[i]) << std::endl;
    }


    return 0;

    //GRAPHICS
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
}
