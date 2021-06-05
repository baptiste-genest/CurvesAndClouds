#include "matrix_functions.h"

cnc::cmat cnc::algo::complexify(const cnc::mat& M){
    uint h = M.rowNum(),w = M.colNum();
    cnc::cmat R(h,w);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            R(i,j) = complex_scalar(M(i,j));
    return R;
}


cnc::mat cnc::algo::Re(const cnc::cmat& M){
    uint h = M.rowNum(),w = M.colNum();
    cnc::mat R(h,w);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            R(i,j) = M(i,j).real();
    return R;
}

cnc::mat cnc::algo::Im(const cnc::cmat& M){
    uint h = M.rowNum(),w = M.colNum();
    cnc::mat R(h,w);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            R(i,j) = M(i,j).imag();
    return R;
}

std::pair<cnc::mat,cnc::mat> cnc::algo::ReImSum(const cnc::cmat& M){
    return {cnc::algo::Re(M),cnc::algo::Im(M)};
}

cnc::cmat cnc::algo::sqrt(const cnc::cmat& M){
    auto ee = M.lanczos();
    std::vector<cnc::complex_scalar> roots(ee.size());
    for (uint k = 0;k<ee.size();k++){
        roots[k] = std::sqrt(cnc::complex_scalar(ee[k].value));
    }
    cnc::cmat P = chaskal::build_basis(ee);
    cnc::cmat D = chaskal::Diag(roots);
    return P*D*P.transpose();
}


cnc::cmat cnc::algo::cos(const cnc::cmat& M){
    const static uint MAX_ITER = 20;
    uint n = M.rowNum();
    cnc::cmat R = chaskal::Id<cnc::complex_scalar>(n);
    cnc::cmat M2 = M*M,X = R;
    cnc::complex_scalar ifac = 1;
    for (uint k = 0;k<MAX_ITER;k++){
        ifac /= (2*k+2)*(2*k+1);
        X = X*M2;
        R += X*ifac*((k%2) == 0 ? cnc::complex_scalar(-1.0) : cnc::complex_scalar(1.0));
    }
    return R;
}
cnc::mat cnc::algo::cos(const cnc::mat& M){
    return cnc::algo::Re(cnc::algo::cos(cnc::algo::complexify(M)));
}

cnc::cmat cnc::algo::sin(const cnc::cmat& M){
    const static uint MAX_ITER = 20;
    cnc::cmat R = M;
    cnc::cmat X = R,M2 = M*M;
    cnc::complex_scalar ifac = 1;
    for (uint k = 0;k<MAX_ITER;k++){
        ifac /= (2*k+2)*(2*k+3);
        X = X*M2;
        R += X*ifac*((k%2) == 0? cnc::complex_scalar(-1.0) : cnc::complex_scalar(1.0));
    }
    return R;
}

cnc::mat cnc::algo::sin(const cnc::mat& M){
    return cnc::algo::Re(cnc::algo::sin(cnc::algo::complexify(M)));
}

cnc::cmat cnc::algo::exp(const cnc::cmat& M){
    const static uint MAX_ITER = 40;
    cnc::cmat R = chaskal::Id<cnc::complex_scalar>(M.rowNum());
    cnc::cmat X = R;
    cnc::complex_scalar ifac = 1;
    for (uint k = 0;k<MAX_ITER;k++){
        ifac /= (k+1);
        X = X*M;
        R += X*ifac;
    }
    return R;
}

cnc::scalar cnc::algo::det22(const cnc::vec& a,const cnc::vec& b){
    return a(0)*b(1) - a(1)*b(0);
}

cnc::vec cnc::algo::Re(const cnc::cvec& V){
    uint h = V.rowNum();
    cnc::vec R(h);
    for (uint j = 0;j<h;j++)
        R(j) = V(j).real();
    return R;
}


cnc::cmat cnc::algo::sqrt(const cnc::mat &M)
{
    return sqrt(complexify(M));
}
