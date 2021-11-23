#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;

int main()
{
    mat M(3);
    vec x(3);
    M(0,0) = 1;
    M(1,1) = 10;
    M(2,1) = 1;
    for (uint i = 0;i<3;i++)
        M(i,2) = 1;
    std::cout << M << std::endl;

    M.plain_log = true;
    for (int a = 1;a<10;a++)
        for (int b = 1;b<10;b++){
            x(0) = 2*a - 10*b;
            x(1) = 3*a;
            x(2) = 22 - 2*a - b;
            auto y = M.solve(x);
            y.plain_log = true;
            bool ok = true;
            for (uint k = 0;k<3;k++){
                if (y(k) < 0 || y(k) > 10)
                    ok = false;
            }
            if (ok){
                printf("%i %i ",a,b);
                std::cout << y << std::endl;
            }
        }
    return 0;
}
