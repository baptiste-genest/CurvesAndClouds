#ifndef PRECONDITIONNERS_H
#define PRECONDITIONNERS_H

#include "lin_alg.h"
#include "sparse_matrix_builder.h"

namespace cnc {

namespace algo {

namespace preconditionners {

mat Jacobi(const mat& A);
}
class SparsePreconditionners {
public:
    static MatrixMultiplicationChain incompleteCholesky(SMB);
};

}

}

#endif // PRECONDITIONNERS_H
