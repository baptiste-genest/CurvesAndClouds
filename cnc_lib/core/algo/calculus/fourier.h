#ifndef FOURIER_H
#define FOURIER_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "algorithm"
#include "calculus.h"

namespace cnc {

namespace algo {

template<class T>
std::vector<T> offset(const std::vector<T>& X,uint s){
    uint N = X.size()-s;
    std::vector<T> R(N);
    for (uint k = 0;k<N;k++)
        R[k] = X[k+s];
    return R;
}

inline uint get_upper_pow2(uint x);



namespace calculus {

std::vector<complex_scalar> FFT(const std::vector<scalar>& X);
std::vector<complex_scalar> FFT(const std::vector<complex_scalar>& X);
std::vector<complex_scalar> FFTP2(const std::vector<cnc::complex_scalar>& X,uint N,uint stride);
std::vector<complex_scalar> iFFT(const std::vector<complex_scalar>& X);
std::vector<scalar> R_iFFT(const std::vector<complex_scalar>& X);

/*
std::vector<complex_scalar> FFT2(const std::vector<scalar>& X,uint w,uint h);
std::vector<complex_scalar> iFFT2(const std::vector<complex_scalar>& X,uint w,uint h);
std::vector<scalar> R_iFFT2(const std::vector<complex_scalar>& X);
*/



}

}

}

#endif // FOURIER_H
