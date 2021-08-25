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

std::pair<cnc::mat, cnc::mat> cnc::algo::set_known_variables(const cnc::mat &M, const std::vector<uint> &id)
{
    if (!std::is_sorted(id.begin(),id.end()))
        throw Cnc_error("Coords of known variables must be sorted");
    uint m = id.size();
    uint N = M.rowNum();
    uint n = N - m;
    mat M1(n),M2(n,m);
    auto inM1 = [id] (uint i,uint j) {
        return !belong<uint>(id,i) && !belong<uint>(id,j);
    };
    auto inM2 = [id] (uint i,uint j) {
        return belong<uint>(id,i) && !belong<uint>(id,j);
    };
    uint M1j = 0,M2j = 0;
    for (uint j = 0;j<N;j++){
        uint M1i = 0,M2i = 0;
        for (uint i = 0;i<N;i++){
            if (inM1(i,j)){
                M1(M1i,M1j) = M(i,j);
                M1i++;
            }
            else if (inM2(i,j)){
                M2(M2i,M2j) = M(i,j);
                M2i++;
            }
        }
        if (!belong(id,j)){
            M2j++;
            M1j++;
        }
    }

    return {M1,M2};
}

cnc::vec cnc::algo::solve_for_kernel_with_known_variables(const cnc::mat &M, const std::vector<uint> &id, const std::vector<cnc::scalar> &v,scalar eps)
{
    if (id.size() != v.size())
        throw Cnc_error("must have as much values as known variables");
    auto P = set_known_variables(M,id);
    vec B(v),X(M.rowNum());
    vec Y = P.second*B*(-1);
    auto x = P.first.solve(Y,eps);
    uint i = 0,j=0;
    for (uint k = 0;k<M.rowNum();k++){
        if (belong(id,k)){
            X(k) = v[i];
            i++;
        }
        else{
            X(k) = x(j);
            j++;
        }
    }
    return X;
}
