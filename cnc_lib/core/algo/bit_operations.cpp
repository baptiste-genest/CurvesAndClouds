#include "bit_operations.h"


uint cnc::algo::cycle_left(uint x, uint bit_width,uint n)
{
    if (n == 0)
        return x;
    uint N = 1<<bit_width;
    return cycle_left(((x<<1)%N) + (x>>(bit_width-1)),bit_width,n-1);
}

uint cnc::algo::cycle_right(uint x, uint bit_width,uint n)
{
    if (n == 0)
        return x;
    return cycle_right((x>>1)+(x%2)*(1<<(bit_width-1)),bit_width,n-1);
}

int cnc::algo::find_cyclic_permutation(uint a, uint b, uint bit_width)
{
    for (int k = 0;k<int(bit_width);k++){
        if (a == b)
            return k;
        else
            b = cycle_left(b,bit_width);
    }
    return -1;
}

bool cnc::algo::get_bit(uint x, uint n) {
    return (x>>n)%2;
}
