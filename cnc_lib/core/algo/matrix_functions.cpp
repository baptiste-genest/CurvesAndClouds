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

std::array<cnc::scalar,3> cnc::algo::symetric_3x3_eigenvalue_solver(const mat &A)
{
    std::array<cnc::scalar,3> eig;
    scalar p1 = sq(A(0,1)) + sq(A(0,2)) + sq(A(1,2));
    if (std::abs(p1) < 1e-10){
       // A is diagonal.
       eig[0] = A(1,1);
       eig[1] = A(2,2);
       eig[2] = A(3,3);
    }
    else {
       scalar q = A.trace()/3;               // trace(A) is the sum of all diagonal values
       scalar p2 = sq(A(0,0) - q) + sq(A(1,1) - q) + sq(A(2,2) - q) + 2 * p1;
       scalar p = std::sqrt(p2 / 6);
       mat B = (A - chaskal::Id<scalar>(3) * q) * (1 / p);//     I is the identity matrix
       scalar r = det33(B) / 2;

       // In exact arithmetic for a symmetric matrix  -1 <= r <= 1
       // but computation error can leave it slightly outside this range.
       scalar phi;
       if (r <= -1)
          phi = M_PI / 3;
       else if (r >= 1)
          phi = 0;
       else
          phi = std::acos(r) / 3;

       // the eigenvalues satisfy eig3 <= eig2 <= eig1
       eig[0] = q + 2 * p * std::cos(phi);
       eig[2] = q + 2 * p * std::cos(phi + (2*M_PI/3));
       eig[1] = 3 * q - eig[0] - eig[2];//    since trace(A) = eig1 + eig2 + eig3;
    }
    return eig;
}

cnc::scalar cnc::algo::det33(const mat &A)
{
    const scalar&  a  = A( 0 , 0 );
    const scalar&  b  = A( 1 , 0 );
    const scalar&  c  = A( 2 , 0 );
    const scalar&  d  = A( 0 , 1 );
    const scalar&  e  = A( 1 , 1 );
    const scalar&  f  = A( 2 , 1 );
    const scalar&  g  = A( 0 , 2 );
    const scalar&  h  = A( 1 , 2 );
    const scalar&  i  = A( 2 , 2 );
    return a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g);
}

QVector3D cnc::algo::symetric_3x3_eigenvector(const cnc::mat &A, cnc::scalar eigenvalue)
{
    QVector3D row0(A(0,0)-eigenvalue,A(1,0),A(2,0));
    QVector3D row1(A(0,1),A(1,1)-eigenvalue,A(2,1));
    QVector3D row2(A(0,2),A(1,2),A(2,2)-eigenvalue);
    QVector3D r0xr1 = QVector3D::crossProduct(row0,row1);
    QVector3D r0xr2 = QVector3D::crossProduct(row0,row2);
    QVector3D r1xr2 = QVector3D::crossProduct(row1,row2);
    scalar d0 = r0xr1.lengthSquared();
    scalar d1 = r0xr2.lengthSquared();
    scalar d2 = r1xr2.lengthSquared();
    scalar dmax = d0;
    int imax = 0;
    if (d1 > dmax) {
        imax = 1;
        dmax = d1;
    }
    if (d2 > dmax) {
        imax = 2;
    }
    if (imax == 0)
        return r0xr1.normalized();
    if (imax == 1)
        return r0xr2.normalized();
    return r1xr2.normalized();
}

cnc::scalar cnc::algo::det33(const cnc::vec &a, const cnc::vec &b, const cnc::vec &c)
{
    mat A(3);
    vec X[3] = {a,b,c};
    for (uint i = 0;i<3;i++){
        for (uint j = 0;j<3;j++){
            A(i,j) = X[i](j);
        }
    }
    return det33(A);
}

cnc::vec cnc::algo::cross(const vec &u, const vec &v)
{
    vec x(3);
    x(0) = u(1)*v(2)-u(2)*v(1);
    x(1) = u(2)*v(1)-u(0)*v(2);
    x(2) = u(0)*v(1)-u(1)*v(0);
    return x;
}
