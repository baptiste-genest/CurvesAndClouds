#include "complex_analysis.h"

std::vector<cnc::complex_scalar> cnc::algo::calculus::complex::roots_of_unity(uint n,scalar offset)
{
    std::vector<cnc::complex_scalar> R(n);
    scalar th = M_PI*2/n;
    for (uint k = 0;k<n;k++)
        R[k] = std::polar<scalar>(1.,th*k + offset);
    return R;
}
