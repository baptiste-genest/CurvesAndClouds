#include "preconditionners.h"

cnc::mat cnc::algo::preconditionners::Jacobi(const cnc::mat &A)
{
    uint N = A.rowNum();
    mat J(N);
    for (uint i = 0;i<N;i++)
        if (std::abs(A(i,i)) > 1e-10)
            J(i,i) = 1.0/A(i,i);
    return J;
}
