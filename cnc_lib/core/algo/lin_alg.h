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

#include "../matrix.h"
#include "../matrix_errors.h"

namespace cnc {

/**
 * @brief vec N dimensional vector
 */
typedef chaskal::Vector<float> vec;
/**
 * @brief mat NxM matrix
 */
typedef chaskal::Matrix<float> mat;

}
#endif // LIN_ALG_H
