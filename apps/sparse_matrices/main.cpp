#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;


int main()
{
    mat D(4,4);
    smat S(4,4);
    S.add_in_row(3,1.f);D(3,0) = 1.f;
    S.new_row();
    S.add_in_row(1,2.f);D(1,1) = 2.f;
    S.new_row();
    S.add_in_row(0,3.f);D(0,2) = 3.f;
    S.new_row();
    S.add_in_row(2,4.f);D(2,3) = 4.f;
    S.end_construct();
    //S.print();
    std::vector<uint> id({0,1,3});
    set_known_variables(S,id).first.print();
    std::cout << set_known_variables(D,id).first << std::endl;
    return 0;
}
