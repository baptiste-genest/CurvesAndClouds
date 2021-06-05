#ifndef MATRIX_ALGO_H
#define MATRIX_ALGO_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "matrix.h"

namespace chaskal {

template<class T>
std::pair<Matrix<T>,Matrix<T>> get_polar_decomposition(const Matrix<T>& M) {
    Matrix<T> S2 = M.transpose()*M;
    auto ee = S2.lanczos();
    std::vector<T> sqrt_diag(ee.size()),inv(ee.size());
    for (uint k = 0;k<ee.size();k++){
        sqrt_diag[k] = std::sqrt(ee[k].value);
        inv[k] = 1/sqrt_diag[k];
    }

    Matrix<T> D = Diag(sqrt_diag);
    Matrix<T> P = build_basis(ee);
    Matrix<T> S = P*D*P.transpose();
    Matrix<T> R = M*P*Diag(inv)*P.transpose();
    return {R,S};
}

}


#endif // MATRIX_ALGO_H
