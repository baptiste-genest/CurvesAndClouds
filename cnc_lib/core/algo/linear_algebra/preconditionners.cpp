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

cnc::sparse::MatrixMultiplicationChain cnc::algo::SparsePreconditionners::incompleteCholesky(cnc::sparse::SMB A)
{
    uint n = A.height();
    A.S = false;
    for (uint k = 0;k<n;k++){
        A(k,k) = std::sqrt(A(k,k));
        for (uint i = k+1;i<n;i++)
            if (!A.isNull(k,i))
                A(k,i) = A(k,i)/A(k,k);
        for (uint j = k+1;j<n;j++){
            for (uint i = j;i<n;i++)
                if (!A.isNull(j,i))
                    A(j,i) = A(j,i) - A(k,i)*A(k,j);
        }
    }
    std::vector<smatbuilder::index> to_delete;
    for (const auto& slot : A.values)
        if (slot.first.first > slot.first.second)
            to_delete.push_back(slot.first);
    for (const auto& index : to_delete)
        A.values.erase(index);
    return {{A,MatrixType::lower},{A.transpose(),MatrixType::upper}};
}
