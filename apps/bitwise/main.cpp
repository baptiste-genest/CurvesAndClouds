#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;


int main()
{
    uint a = 5,b=cycle_left(a,4,2),L = 4;
    std::cout << a << ' ' << cycle_right(b,4,find_cyclic_permutation(a,b,L)) << std::endl;
    return 0;
}
