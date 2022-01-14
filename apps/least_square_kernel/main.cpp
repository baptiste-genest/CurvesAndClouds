#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;


int main()
{
    mat M(2,2,{1,1,1,1});
    mat A(3,2,{1,1,1,1,1,0});
    auto At = A.transpose();
    vec b({0.,0.,1.});
    std::cout << A << std::endl;
    std::cout << At*A << std::endl;
    std::cout << At*b << std::endl;
    auto x = solve22(At*A,At*b);
    std::cout << x << std::endl;
    std::cout << M*x << std::endl;
    return 0;
}
