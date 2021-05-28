/**
  * @file algo.h
  *
  * list of useful algorithms for general purposes
  *
*/
#ifndef ALGO_H
#define ALGO_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <functional>
#include <algorithm>
#include <vector>

#include "cnc_types.h"
#include "core/algo/calculus/calculus.h"
#include "lin_alg.h"

namespace cnc {

/**
 * @brief Namespace containing all the algorithms relative to generic computation
 */
namespace algo {

/**
 * @brief get_min_max_range
 * @param X list of values
 * @return range of [min(X),max(X)]
 */
range get_min_max_range(const std::vector<scalar>& X);

std::vector<range> get_min_max_range(const std::vector<vec>& X);

/**
 * @brief get_index_lin_space
 * @param a index start
 * @param b index end
 * @param step between each index
 * @return [a,a+step,...,b]
 */
std::vector<int> get_index_lin_space(int a,int b,int step = 1);

/**
 * @brief check_unicity
 * @param v list of real numbers
 * @param eps equality condition on real numbers
 * @return true  if two numbers are equal (are their diff is less than eps)
 */
bool check_unicity(const std::vector<scalar>& v,scalar eps = 1e-8);

/**
 * @brief scale_range scales the difference between the two bounds a range
 * @param r range to scale
 * @param s scale factor
 */
void scale_range(range& r,scalar s);

/**
 * @brief sort a range to make sure that the first bound is smaller than the second
 * @param r range to sort
 * @return sorted range
 */
range sort(const range& r );

bool is_sorted(const range& r);

/**
 * @brief sign sign function
 * @param x
 * @return -1 if x < 0, 1 else
 */
int sign(scalar x);

enum set_type {
    empty,
    discrete,
    continuous
};

range inter(const range& A,const range& B);
set_type inter_type(const range& A,const range& B);
void extend_range(range& A,const range& B);


}

}
#endif // ALGO_H
