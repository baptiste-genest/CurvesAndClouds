#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;

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

int main()
{
    smat A = build_sparse_2D_laplacian(65,118);
    vec b(65*118);
    A*b;
    return 0;
}
