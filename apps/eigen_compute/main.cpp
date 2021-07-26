#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;


int main()
{
    mat A(4,4,{
              1,1,1,1,
              1,5,5,5,
              1,5,14,14,
              1,5,14,15
          });
    mat L = A.LL();
    vec Y({1.,2.,3.,4.});
    std::cout << A << std::endl;
    std::cout << L << std::endl;
    std::cout << L.transpose()*L << std::endl;
    vec Xp = L.transpose().lower_solve(Y);
    vec X = L.upper_solve(Xp);
    std::cout << X << std::endl;
    std::cout << A*X << std::endl;
    std::cout << A.solve(Y) << std::endl;

    return 0;
}
