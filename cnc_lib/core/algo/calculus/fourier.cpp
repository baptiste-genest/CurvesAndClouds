#include "fourier.h"

std::vector<cnc::complex_scalar> cnc::algo::calculus::FFT(const std::vector<cnc::complex_scalar>& X)
{
    uint N = X.size();
    scalar L = std::log2(N);
    uint padding = N;
    std::vector<cnc::complex_scalar> tmp = X;
    if (L-int(L) > 1e-6){ // if N isn't a power of 2,pad with zeros until it is
        padding = std::pow(2,std::ceil(L));
        tmp.resize(padding);
        std::cout << "padding to " << padding << std::endl;
    }
    std::vector<cnc::complex_scalar> Xhat(padding);
    Xhat = FFTP2(tmp,padding,1);
    return Xhat;
}

std::vector<cnc::complex_scalar> cnc::algo::calculus::FFTP2(const std::vector<cnc::complex_scalar>& X,uint N,uint stride)
{
    constexpr static scalar TWOPI = 2*M_PI;
    if (N == 1)
        return {X[0]};
    const scalar twiddle = -TWOPI/N;
    std::vector<complex_scalar> R(N);
    uint H = N>>1;
    std::vector<complex_scalar> H1 = FFTP2(X,H,stride<<1);
    std::vector<complex_scalar> H2 = FFTP2(offset(X,stride),H,stride<<1);
    for (uint k = 0;k<H;k++){
        R[k] = H1[k];
        R[k+H] = H2[k];
    }
    for (int k = 0;k<int(H);k++){
        complex_scalar p,q;
        p = R[k];
        q = std::polar(scalar(1),k*twiddle)*R[k+H];
        R[k] = p + q;
        R[k+H] = p - q;
    }
    return R;
}

std::vector<cnc::complex_scalar> cnc::algo::calculus::iFFT(const std::vector<cnc::complex_scalar> &X)
{
    scalar scale = scalar(1)/X.size();
    std::vector<complex_scalar> Xbar = X;
    for (complex_scalar& x : Xbar)
        x = std::conj(x);
    Xbar = FFT(Xbar);
    for (complex_scalar& x : Xbar)
        x = std::conj(x)*scale;
    return Xbar;
}

std::vector<cnc::complex_scalar> cnc::algo::calculus::FFT(const std::vector<cnc::scalar> &X)
{
    std::vector<cnc::complex_scalar> CX(X.size());
    for (uint k = 0;k<X.size();k++)
        CX[k] = X[k];
    return FFT(CX);
}

std::vector<cnc::scalar> cnc::algo::calculus::R_iFFT(const std::vector<cnc::complex_scalar> &X)
{
    uint N = X.size();
    std::vector<complex_scalar> CX = iFFT(X);
    std::vector<scalar> R(N);
    for (uint k = 0;k<N;k++)
        R[k] = CX[k].real();
    return R;
}

inline uint cnc::algo::get_upper_pow2(uint x) {
    return std::pow(2,std::ceil(std::log2(x)));
}
