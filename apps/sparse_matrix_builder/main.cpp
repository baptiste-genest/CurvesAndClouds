#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;

int main()
{
    uint N = 3;
    SMB H(N,true);
    vec b(N);
    for (uint i = 0;i<N;i++){
        H(i,i) = i+1;
        b(i) = 1;
    }
    SMB B(N,true);
    B(0,0) = 1;
    B(1,0) = 5;
    B(2,0) = 3;
    SMB C = B;
    C += H;
    H.print();
    B.print();
    C.print();
    smat A(C);
    A.print();

    return 0;
}
