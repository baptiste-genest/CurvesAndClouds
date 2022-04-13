#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;

mat submat(const mat& A){
    auto w = A.colNum();
    auto h = A.rowNum();
    mat S(h-1,w-1);
    for (uint i = 0;i<w-1;i++)
        for (uint j = 0;j<h-1;j++)
            S(i,j) = A(i+1,j+1);
    return S;
}

mat reflexion(const vec& x){
    vec e1(x.rowNum());
    e1(0) = 1;
    auto v = (x - e1*x.norm()).normalize();
    return chaskal::Id<scalar>(x.rowNum()) - v.transpose()*v*2;
}

vec getcol(const mat& A){
    vec a(A.rowNum());
    for (uint j = 0;j<A.rowNum();j++)
        a(j) = A(0,j);
    return a;
}

mat fill(const mat& S,uint w,uint h){
    mat F(h,w);

    return F;
}

void householder(const mat& A){

}

int main()
{
    mat A(3,2);
    A(0,0) = 1.;
    A(1,0) = 2.;
    A(0,1) = 3.;
    A(1,1) = 4.;
    std::cout << A << std::endl;
    std::cout << submat(A) << std::endl;

    return 0;
}
