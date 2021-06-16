/**
  * @file random_var.h
  *
  * All algorithms and types related to simulating random variables
  *
*/
#ifndef RANDOM_VAR_H
#define RANDOM_VAR_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "core/algo/algo.h"
#include "core/algo/statistics/statistics.h"

#include "core/matrix.h"

#include <random>

namespace cnc {

namespace algo {

namespace stat {

/**
 * @brief namespace containing random variables related functions
 */
namespace random_var {

/**
 * @brief random_scalar return a random scalar between a and b
 * @param a lower bound
 * @param b upper bound
 * @return
 */
scalar random_scalar(scalar a,scalar b);
complex_scalar random_complex_scalar(scalar a,scalar b);


/**
 * @brief sample_normal_random_vectors builds a point cloud from a sample of a random normal vector
 * @param means means of the componants of the vector
 * @param var standard deviation of the componants of the vector
 * @param N number of samples
 * @return sample cloud
 */
cloud sample_normal_random_vectors(const std::vector<scalar>& means,const std::vector<scalar>& var,uint N);

/**
 * @brief sample_centered_reduced_gaussian_vector sample a gaussian vector following
 * @param dim dimension of the vector
 * @param N number of samples
 * @return cloud
 */
cloud sample_centered_reduced_gaussian_vector(uint dim,uint N);

/**
 * @brief sample_gaussian_vector_by_PC gen samples of a gaussian vector generated according to axis and given variance and mean
 * @param pc_axis family of vectors composing axis, MUST BE ORTHONORMAL
 * @param var variance associated with each axis
 * @param mean mean of the vector
 * @param N number of samples
 * @return samples
 */
cloud sample_gaussian_vector_by_PC(const std::vector<vec>& pc_axis,const std::vector<scalar>& var,const vec& mean,uint N);

/**
 * @brief sample_vector_on_unit_sphere generate a point cloud with coords between [-1,1]
 * @param dim dimension of the points
 * @param N number of points
 * @return samples cloud
 */
cloud sample_vector_on_unit_sphere(uint dim,uint N);

/**
 * @brief sample_uniform_in_square generate a point cloud with coords between [-half_width,half_width]
 * @param dim dimension of the points
 * @param half_width range of the coords
 * @param N number of points
 * @return samples cloud
 */
cloud sample_uniform_in_square(uint dim,scalar half_width,uint N);

mat random_mat(scalar lb,scalar ub,uint n);
mat random_mat(scalar lb,scalar ub,uint h,uint w);

cmat random_complex_mat(scalar lb,scalar ub,uint n);
cmat random_complex_mat(scalar lb,scalar ub,uint h,uint w);

}

}

}

}

#endif // RANDOM_VAR_H
