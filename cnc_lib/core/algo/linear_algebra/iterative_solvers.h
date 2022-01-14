#ifndef ITERATIVE_SOLVERS_H
#define ITERATIVE_SOLVERS_H
#include "preconditionners.h"
#include "sparse_matrix_builder.h"
#include "../statistics/random_var.h"

namespace cnc {
namespace algo {
using eigen_mode = chaskal::eigen_pair<scalar>;

class IterativeSolvers
{
public:
    static vec ConjugateGradient(const smat& A,const vec& b,const vec& x0,scalar eps,bool debug = false);
    static vec ConjugateGradient(const mat& A,const vec& b,const vec& x0,scalar eps,bool debug = false);
    static eigen_mode InversePowerMethod(const mat& A,scalar eps,bool debug = false);
    static eigen_mode PowerMethod(const mat& A,scalar eps,bool debug = false);
    static eigen_mode PowerMethod(const mat& A,const vec& x0,scalar eps,bool debug = false);
    static vec PreconditionnedConjugateGradient(const smat& A,const vec& b,const MatrixMultiplicationChain& C,const vec& x0,scalar eps,bool debug = false);
};

}
}


#endif // ITERATIVE_SOLVERS_H
