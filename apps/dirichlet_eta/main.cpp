#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;

cscalar eta(const cscalar& tau){
    static cscalar I(0,1);
    cscalar E = std::exp(M_PI/12.*I*tau);
    for (uint i = 1;i<=5;i++){
        E *= 1.-std::exp(M_PI*2.*i*I*tau);
    }
    return E;
}

int main()
{
    std::cout << eta(std::polar(5.,0.1)) << std::endl;

    return 0;
}
