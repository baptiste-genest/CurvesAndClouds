#include "finite_differences.h"

cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::build_derivative(const cnc::algo::calculus::scalar_function_1D &f, float dx)
{
    float idx = 1.f/dx;
    return [f,dx,idx] (float x){
        return (f(x+dx)-f(x))*idx;
    };
}

cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::build_second_derivative(const cnc::algo::calculus::scalar_function_1D &f, float dx)
{
    float idx2 = 1.f/(dx*dx);
    return [f,dx,idx2] (float x) {
        return (-2.f*f(x) +f(x-dx) + f(x+dx))*idx2;
    };
}
