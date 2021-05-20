#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "cnc_error.h"

namespace cnc {

namespace algo {

uint cycle_left(uint x,uint bit_width,uint n = 1);
uint cycle_right(uint x,uint modulus,uint n = 1);
int find_cyclic_permutation(uint a,uint b,uint bit_width);
bool get_bit(uint x,uint n);

}

}


#endif // BIT_OPERATIONS_H
