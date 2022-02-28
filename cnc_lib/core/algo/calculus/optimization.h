/**
  * @file optimization.h numerical methods to find f(x) = 0 or solve minimization problems
  */
#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
#include "calculus.h"
#include "vector_calculus.h"
#include <algorithm>

namespace cnc {

namespace algo {

namespace calculus {

/**
 * @brief algorithms to find zeros of functions or extremas
 */
namespace optimization {

/**
 * @brief newton_method_1D
 * @param f function to find the zero
 * @param df derivative of f
 * @param x0 where to start the iterations
 * @param eps accepted error
 * @param max_iter maximum number of iterations
 * @return zero approximation
 */
scalar newton_method_1D(const scalar_function_1D& f,const scalar_function_1D& df,scalar x0,scalar eps = 1e-5,uint max_iter = 1000);

/**
 * @brief dichotomia
 * @param f
 * @param a
 * @param b
 * @param eps
 * @param max_iter
 * @return
 */
scalar min_dichotomia(const scalar_function_1D& f, scalar a,scalar b,scalar eps,uint max_iter);

inline scalar_function_1D build_descent_function(const scalar_function& f,const vec& x,const vec& d);

vec gradient_descent(const scalar_function& f,const vec& x0,scalar eps = 1e-5,uint max_iter = 1000);
vec gradient_descent_fixed_step(const scalar_function& f,const vec& x0,scalar h,scalar eps = 1e-5,scalar dx = 1e-5,uint max_iter = 1000);
vec gradient_descent_fixed_step(const vector_function& grad,const vec& x0,scalar h,scalar eps = 1e-5,uint max_iter = 1000);
vec gradient_descent_adaptive_step(const vector_function& grad,const vec& x0,scalar h,scalar eps = 1e-5,uint max_iter = 1000);

std::vector<vec> tracked_gradient_descent_adaptive_step(const vector_function& grad,const vec& x0,scalar h,scalar eps = 1e-5,uint max_iter = 1000);

template<class T>
T argmin(const std::function<scalar(const T&)>& f,const std::vector<T>& X){
    std::vector<scalar> values(X.size());
    for (uint k = 0;k<X.size();k++)
        values[k] = f(X[k]);
    return X[std::distance(values.begin(),std::min_element(values.begin(),values.end()))];
}

}

}

}

}

#endif // OPTIMIZATION_H
