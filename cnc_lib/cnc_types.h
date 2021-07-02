/**
  * @file cnc_types.h defines basic cnc_types
  */
#ifndef CNC_TYPES_H
#define CNC_TYPES_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <utility>
#include <iostream>
#include <complex>

typedef unsigned int uint;

/**
 * @brief The namespace to use all the functions of Curves&Clouds
 */
namespace cnc {

#if PRECISION == FLOAT
typedef float scalar;
#elif PRECISION == DOUBLE
typedef double scalar;
#elif PRECISION == LONG_DOUBLE
typedef double scalar;
#else
typedef float scalar;
#endif

typedef std::complex<scalar> complex_scalar;
typedef std::complex<scalar> cscalar;

/**
 * @brief A pair of float
 */
typedef std::pair<scalar,scalar> range;

enum axis {
    x_axis,
    y_axis
};
}
#endif // CNC_TYPES_H
