#include <curvesandcloud.h>

using namespace cnc;

//algo_GUI::texture_function get_paster(const )

smat build_sparse_2D_laplacian(uint w,uint h){
    int A = w*h;
    smat L(A);
    for (int j = 0;j<A;j++){

        int y = j/w, x = j%w;
        int nb_non_null = 0;
        if (y>0)
            nb_non_null++;
        if (x>0)
            nb_non_null++;
        if (x+1<w)
            nb_non_null++;
        if (y+1<h)
            nb_non_null++;
        if (y>0)
            L.add_in_row(j - w,1);
        if (x>0)
            L.add_in_row(j - 1,1);
        L.add_in_row(j,-nb_non_null);
        if (x+1<w)
            L.add_in_row(j + 1,1);
        if (y+1<h)
            L.add_in_row(j + w,1);
        L.new_row();
    }
    L.end_construct();
    return L;
}

std::vector<uint> compute_boundary_id(uint w,uint h){
    std::vector<uint> B;
    for (uint i = 0;i<w;i++)
        B.push_back(i);
    for (uint j = 1;j<h-1;j++){
        B.push_back(j*w);
        B.push_back(j*w+ w - 1);
    }
    for (uint i = 0;i<w;i++)
        B.push_back(w*(h-1) + i);

    return B;
}

std::function<uint(uint)> build_index_mapper(uint w1,uint h1,uint w2,uint h2,uint lc,uint bc){
    return [w1,h1,w2,h2,lc,bc] (uint i2) {
        uint x = i2 % w2 + lc;
        uint y = i2 / w2 + bc;
        return y*w1 + x;
    };
}

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    PlotWindow W; W.resize(500,500);
    auto T = W.add_tab("copy paste");

    uint w1,h1;
    auto G = algo::load_img_as_vec("../../data/rushmore.jpg",w1,h1);
    std::cout << w1 << ' ' << h1 << std::endl;

    uint w2,h2;
    auto F = algo::load_img_as_vec("../../data/face_small.jpg",w2,h2);
    std::cout << w2 << ' ' << h2 << std::endl;
    uint lc = 40,bc = 30;
    T->add_frame()->add_layer()->new_figure_from_texturing(w1,h1,[G,F,w1,h1,w2,h2,lc,bc] (uint i,uint j){

        int x = int(i) - lc;int y = int(j) - bc;
        QColor C;
        if (x< w2 && x >= 0 && y <h2 && y >= 0){
            uint id = y*w2 + x;
            C.setRedF(F[0](id));
            C.setGreenF(F[1](id));
            C.setBlueF(F[2](id));
        }
        else {
            uint id = j*w1 + i;
            C.setRedF(G[0](id));
            C.setGreenF(G[1](id));
            C.setBlueF(G[2](id));
        }
        return C;
    });


    smat Lap = build_sparse_2D_laplacian(w2,h2);
    std::vector<uint> del_omega = compute_boundary_id(w2,h2);

    std::cout << std::is_sorted(del_omega.begin(),del_omega.end()) << std::endl;
    auto KV = algo::set_known_variables(Lap,del_omega);
    uint N = del_omega.size();
    auto M = build_index_mapper(w1,h1,w2,h2,lc,bc);
    for (uint k = 0;k<3;k++){
        std::vector<scalar> v(N);
        for (uint i = 0;i<N;i++)
            v[i] = G[k](M(del_omega[i])) - F[k](del_omega[i]);
        F[k] = algo::solve_for_kernel_with_known_variables(KV,w2*h2,del_omega,v,1e-10) + F[k];
    }

    T->add_frame()->add_layer()->new_figure_from_texturing(w1,h1,[G,F,w1,h1,w2,h2,lc,bc] (uint i,uint j){
        int x = int(i) - lc;int y = int(j) - bc;
        QColor C;
        if (x< w2 && x >= 0 && y <h2 && y >= 0){
            uint id = y*w2 + x;
            C.setRedF(F[0](id));
            C.setGreenF(F[1](id));
            C.setBlueF(F[2](id));
        }
        else {
            uint id = j*w1 + i;
            C.setRedF(G[0](id));
            C.setGreenF(G[1](id));
            C.setBlueF(G[2](id));
        }
        return C;
    });


    W.show();
    return a.exec();
}
