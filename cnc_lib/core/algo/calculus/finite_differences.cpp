#include "finite_differences.h"

cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::build_derivative(const cnc::algo::calculus::scalar_function_1D &f, scalar dx)
{
    scalar idx = 1.f/dx;
    return [f,dx,idx] (scalar x){
        return (f(x+dx)-f(x))*idx;
    };
}

cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::build_second_derivative(const cnc::algo::calculus::scalar_function_1D &f, scalar dx)
{
    scalar idx2 = 1.f/(dx*dx);
    return [f,dx,idx2] (scalar x) {
        return (-2.f*f(x) +f(x-dx) + f(x+dx))*idx2;
    };
}
