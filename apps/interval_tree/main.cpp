#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::data_structure;

using intree = IntervalTree<int>;

int main()
{
    range r = {-2.,3.};
    intree T;
    T.insert(r,2);
    auto root = T.insert({-1.,3.},2);
    T.print();
    auto I = T.checkOverlap(4);
    std::cout << I.size() << std::endl;
    return 0;
}
