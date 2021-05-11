#include "integration_scheme.h"



cnc::algo::calculus::scheme cnc::algo::calculus::integration_scheme::left_rectangle(const cnc::algo::calculus::scalar_function_1D &f)
{
    return [f] (scalar a,scalar b){
        return (b-a)*f(a);
    };
}

cnc::algo::calculus::scheme cnc::algo::calculus::integration_scheme::right_rectangle(const cnc::algo::calculus::scalar_function_1D &f)
{
    return [f] (scalar a,scalar b){
        return (b-a)*f(b);
    };
}

cnc::algo::calculus::scheme cnc::algo::calculus::integration_scheme::mid_point(const cnc::algo::calculus::scalar_function_1D &f)
{
    return [f] (scalar a,scalar b){
        return (b-a)*f((a+b)*0.5f);
    };
}

cnc::algo::calculus::scheme cnc::algo::calculus::integration_scheme::trapezoidal(const cnc::algo::calculus::scalar_function_1D &f)
{
    return [f] (scalar a,scalar b){
        return 0.5f*(b-a)*(f(a)+f(b));
    };
}

cnc::algo::calculus::scheme cnc::algo::calculus::integration_scheme::simpson(const cnc::algo::calculus::scalar_function_1D &f)
{
    return [f] (scalar a,scalar b){
        return ((b-a)/6.f)*(f(a)+4.f*f((a+b)*0.5f)+f(b));
    };

}

cnc::scalar cnc::algo::calculus::integrate(const cnc::algo::calculus::scheme &s, const cnc::algo::calculus::nodes &n)
{
    scalar J = 0;

    for (uint i = 0;i<n.size()-1;i++)
        J += s(n[i],n[i+1]);

    return J;
}
