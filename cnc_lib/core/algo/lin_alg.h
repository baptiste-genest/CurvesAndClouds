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

#include "../../cnc_types.h"
#include "../matrix.h"
#include "../matrix_algo.h"
#include "../matrix_errors.h"

namespace cnc {

/**
 * @brief vec N dimensional vector
 */
typedef chaskal::Vector<scalar> vec;
typedef chaskal::Vector<complex_scalar> cvec;
/**
 * @brief mat NxM matrix
 */
typedef chaskal::Matrix<scalar> mat;
typedef chaskal::Matrix<complex_scalar> cmat;

/*
vec vec2(scalar x,scalar y);
vec vec3(scalar x,scalar y);
*/

}
#endif // LIN_ALG_H
