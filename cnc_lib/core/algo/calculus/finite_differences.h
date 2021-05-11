/** @file finite_differences.h
  * definitions of basic finites differences schemes for numerical integration/DE solving
  */
#ifndef FINITE_DIFFERENCES_H
#define FINITE_DIFFERENCES_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "calculus.h"
namespace cnc {

namespace algo {

namespace calculus {

/**
 * @brief build_derivative build a function to evaluate the derivative of a given scalar function (f(x+dx) - f(x))/dx
 * @param f given function
 * @param dx space step (better be small)
 * @return derivative of the function
 */
scalar_function_1D build_derivative(const scalar_function_1D& f,float dx = 1e-6);

/**
 * @brief build_second_derivative build a function to evaluate the second derivative of a given scalar function (2*f(x) - f(x+dx) - f(x-dx))/dx^2
 * @param f given function
 * @param dx space step (better be small)
 * @return derivative of the function
 */
scalar_function_1D build_second_derivative(const scalar_function_1D& f,float dx = 1e-6);


}

}

}

#endif // FINITE_DIFFERENCES_H
