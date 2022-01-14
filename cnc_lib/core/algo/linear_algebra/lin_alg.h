/**
  * @file lin_alg.h define types for linear algebra in CNC
  */
#ifndef LIN_ALG_H
#define LIN_ALG_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "../../../cnc_types.h"
#include "../../matrix.h"
#include "../../matrix_algo.h"
#include "../../matrix_errors.h"

namespace cnc {

/**
 * @brief vec N dimensional vector
 */
using vec = chaskal::Vector<scalar>;
using cvec = chaskal::Vector<complex_scalar>;
/**
 * @brief mat NxM matrix
 */
using mat = chaskal::Matrix<scalar>;
using cmat = chaskal::Matrix<complex_scalar>;

namespace linear_utils {

inline vec vec2(scalar x= 0,scalar y=0){
    return vec({x,y});
}
inline vec vec3(scalar x,scalar y,scalar z){
    return vec({x,y,z});
}

}

/*
vec vec2(scalar x,scalar y);
vec vec3(scalar x,scalar y);
*/

}
#endif // LIN_ALG_H
