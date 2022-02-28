#include "lin_alg.h"

cnc::vec cnc::linear_utils::ones(uint n)
{
    vec O(n);
    for (uint i = 0;i<n;i++)
        O(i) = 1;
    return O;
}

