#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "lin_alg.h"
#include "algo.h"
#include "core/sparse_matrix.h"
#include <QVector3D>

namespace cnc {
namespace algo {

cmat complexify(const mat& M);

mat Re(const cmat& M);
mat Im(const cmat& M);

std::pair<mat,mat> ReImSum(const cmat& M);

cmat sqrt(const cmat& M);
cmat sqrt(const mat& M);

cmat cos(const cmat& M);

mat cos(const mat& M);

cmat sin(const cmat& M);

mat sin(const mat& M);

cmat exp(const cmat& M);

scalar det22(const vec& a,const vec& b);

scalar det33(const mat& A);


vec Re(const cvec& V);

std::pair<mat,mat> set_known_variables(const mat& M,const std::vector<uint>& id);
vec solve_for_kernel_with_known_variables(const mat& M,const std::vector<uint>& id,const std::vector<scalar>& v,scalar eps = 1e-8);

std::array<scalar,3> symetric_3x3_eigenvalue_solver(const mat& A);
QVector3D symetric_3x3_eigenvector(const mat& A,scalar eigenvalue);


}
}

#endif // MATRIX_FUNCTIONS_H
