/**
  * @file calculus.h
  *
  * All algorithms and types related to mathematical computation and analysis of functions
  *
*/
#ifndef CALCULUS_H
#define CALCULUS_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "core/algo/algo.h"
#include "../lin_alg.h"
#include "cnc_error.h"
#include "cnc_types.h"

#include <algorithm>
#include <utility>
#include <cmath>

namespace cnc {

namespace algo {

/**
 * @brief namespace containing all the algorithms relative to calculus
 */
namespace calculus
{

static constexpr float epsilon = 1e-12;

/**
 * @brief nodes datastruct is the basic structure to tell where to integrate/compute
 */
typedef std::vector<float> nodes;

/**
 * @brief scalar_function_1D are real functions of 1 real argument
 */
typedef std::function<float(float)> scalar_function_1D;

/**
 * @brief scalar_function_2D are real functions of 2 real arguments
 */
typedef std::function<float(float,float)> scalar_function_2D;

/**
 * @brief scalar_function are real functions of 1 real-vector argumen^t
 */
typedef std::function<float(const cnc::vec&)> scalar_function;
/**
 * @brief vector_function_2D are real-vector values of 2 real arguments
 */
typedef std::function<cnc::vec(float,float)> vector_function_2D;
/**
 * @brief vector_function are real-vector values of 1 real-vector argument
 */
typedef std::function<cnc::vec(const cnc::vec&)> vector_function;


/**
 * @brief get_lin_space create 1D evenly spaced nodes set from a to b with step dx
 * @param a start
 * @param b end
 * @param dx space step
 * @return vector of nodes
 */
std::vector<float> get_lin_space(float a,float b,float dx);
/**
 * @brief get_lin_space create 1D evenly spaced nodes set in a given range with step dx
 * @param x range
 * @param dx space step
 * @return vector of nodes
 */
std::vector<float> get_lin_space(range x,float dx);

/**
 * @brief get_lin_space create 1D evenly spaced nodes set from a to b with N steps
 * @param a start
 * @param b end
 * @param N number of nodes
 * @return vector of nodes
 */
std::vector<float> get_lin_space(float a,float b,int N);
/**
 * @brief get_lin_space create 1D evenly spaced nodes set in a given range with N steps
 * @param x range
 * @param N number of nodes
 * @return vector of nodes
 */
std::vector<float> get_lin_space(range x,int N);

/**
 * @brief build_1D_linear_interpolator given x will interpolate depending on two closest X nodes corresponding Y values
 * @param X x points coord
 * @param Y y points coord
 * @return 1D function that interpolates value between two closest points of the parameter
 */
scalar_function_1D build_1D_linear_interpolator(const nodes& X,const nodes& Y);

/**
 * @brief build_range_mapper build a scalar function that maps each value in a range into an other one for exemple
 * @param A source range
 * @param B target range
 * @return mapper from A to B
 */
scalar_function_1D build_range_mapper(const range& A,const range& B);

/**
 * @brief sample nodes with values of a given function at given points
 * @param f the function to sample
 * @param X the points where sample
 * @return sample nodes
 */
nodes sample(const scalar_function_1D& f,const nodes& X);

/**
 * @brief build_integration_mesh homemade algorithm to build a better 1D integration mesh than the uniform one
 * @param f
 * @param a
 * @param b
 * @param dx
 * @return
 */
nodes build_integration_mesh(const scalar_function_1D& f, float a,float b,float dx);

/**
 * @brief build_2D_laplacian build a 2D function to evaluation the laplacian at any point of any function
 * @param f function to analyse
 * @param dx space step (better be small)
 * @return function
 */
scalar_function_2D build_2D_laplacian(const scalar_function_2D& f,float dx = 0.01f);

nodes convert_to_nodes(const vec& x);

}

}

}

#endif // CALCULUS_H
