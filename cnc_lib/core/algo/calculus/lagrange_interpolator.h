/**
  * @file lagrange_interpolator.h
  *
  *   implementation of lagrangian interpolation throught newton method
  *
*/
#ifndef LAGRANGE_INTERPOLATOR_H
#define LAGRANGE_INTERPOLATOR_H
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
 * @brief The lagrange_interpolator class is a class used as a functor to to build and use lagrange interpolator efficiently with newton algorithm
 */
class lagrange_interpolator {
    public:
        /**
         * @brief build_lagrange_interpolator builder that will build the polyonomial in newton form
         * @param X
         * @param Y
         */
        lagrange_interpolator(const nodes& X,const nodes& Y);

        /**
         * @brief operator scalar_function_1D convert to a scalar_function to allow use as a regular function (to plot it for exemple)
         */
        operator scalar_function_1D() const;

        /**
         * @brief operator () functor definition, will compute the interpolation value at x
         * @param x value to interpolate
         * @return interpolated value
         */
        scalar operator ()(scalar x) const;

    private:
        uint N;
        nodes X;
        nodes Y;
        std::vector<double> coefs;

        uint nb_computed_coefs;

        struct bracket_info {
            scalar leftmost_x;
            scalar rightmost_x;
            scalar value;
        };

        void divided_difference_algo();
};

/**
 * @brief get_tchebytchev_points get nodes at the roots of tchebytchev points scaled to fit in a given range
 * @param N number of points
 * @param x range of the points
 * @return nodes
 */
nodes get_tchebytchev_points(uint N,const range& x);

}}}

#endif // LAGRANGE_INTERPOLATOR_H
