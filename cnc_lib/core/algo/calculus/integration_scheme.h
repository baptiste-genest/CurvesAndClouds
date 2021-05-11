/**
  * @file integration_scheme.h
  * defines common integration schemes
  */
#ifndef INTEGRATION_SCHEME_H
#define INTEGRATION_SCHEME_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "calculus.h"

namespace cnc {

namespace algo {

namespace calculus {

typedef std::function<scalar(scalar,scalar)> scheme;

/**
 * @brief namespace containing all integration schemes
 */
namespace integration_scheme {

/**
 * @brief left_rectangle build the left rectangle integration scheme : LR(a,b) = f(a)*(b-a)
 * @param f function to integrate
 * @return scheme
 */
scheme left_rectangle(const scalar_function_1D& f);

/**
 * @brief right_rectangle build the right rectangle integration scheme : LR(a,b) = f(b)*(b-a)
 * @param f function to integrate
 * @return scheme
 */
scheme right_rectangle(const scalar_function_1D& f);

/**
 * @brief mid_point build the mid point integration scheme : MP(a,b) = f((a+b)/2)*(b-a)
 * @param f function to integrate
 * @return scheme
 */
scheme mid_point(const scalar_function_1D& f);

/**
 * @brief trapezoidal build the trapezoidal integration scheme : TR(a,b) = (f(a)+f(b))*(b-a)/2
 * @param f function to integrate
 * @return scheme
 */
scheme trapezoidal(const scalar_function_1D& f);

/**
 * @brief simpson build the simpson integration scheme : SM(a,b) = (f(a)+4f((a+b)/2)+f(b))*(b-a)/6
 * @param f function to integrate
 * @return scheme
 */
scheme simpson(const scalar_function_1D& f);

}

/**
 * @brief integrate compute the integral of a function on a set of nodes, given its integration scheme
 * @param s scheme (encapsulate the function to integrate)
 * @param n nodes where integrate
 * @return approximated integral
 */
scalar integrate(const scheme& s,const nodes& n);

}

}

}


#endif // INTEGRATION_SCHEME_H
