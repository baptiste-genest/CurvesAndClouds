#include "lagrange_interpolator.h"
using namespace cnc;


void cnc::algo::calculus::lagrange_interpolator::divided_difference_algo()
{
    std::vector<std::vector<bracket_info>> brackets(N);
    for (uint i = 0;i<N;i++)
        brackets[0].push_back({X[i],X[i],Y[i]});

    bracket_info left,right;

    for (uint j = 0;j<N-1;j++){
        for (uint i = 0;i< N-j-1;i++){
            left = brackets[j][i];
            right = brackets[j][i+1];
            scalar rrm = right.rightmost_x;
            scalar llm = left.leftmost_x;
            brackets[j+1].push_back({llm,rrm,(right.value - left.value)/(rrm-llm)});
        }
    }

    for (uint j = 0;j<N;j++)
        coefs[j] = brackets[j][0].value;
}

cnc::algo::calculus::lagrange_interpolator::lagrange_interpolator(const cnc::algo::calculus::nodes &x, const cnc::algo::calculus::nodes &y)
{
    if (!check_unicity(x))
        throw Cnc_error("x nodes of lagrange interpolation must be distincts");
    if (x.size() != y.size())
        throw Cnc_error("to build lagrange interpolator you must have same number of X and Y nodes");
    X = x; Y=y;
    N = X.size();
    coefs.resize(N);

    divided_difference_algo();
}

cnc::algo::calculus::lagrange_interpolator::operator scalar_function_1D() const
{
    auto& c = coefs;
    auto& Xn = X;
    auto& n = N;
    return [c,Xn,n] (scalar x){
        scalar interpolation = 0;
        for (uint k = 0;k<n;k++){
            scalar f = 1;
            for (uint i = 0;i<k;i++)
                f *= x-Xn[i];
            interpolation += c[k]*f;
        }
        return interpolation;
    };
}

scalar cnc::algo::calculus::lagrange_interpolator::operator()(scalar x) const
{
    scalar interpolation = 0;
    for (uint k = 0;k<N;k++){
        scalar f = 1;
        for (uint i = 0;i<k;i++)
            f *= x-X[i];
        interpolation += coefs[k]*f;
    }
    return interpolation;
}

cnc::algo::calculus::nodes cnc::algo::calculus::get_tchebytchev_points(uint N, const cnc::range &x)
{
    scalar a = x.first,b= x.second,mid = (a+b)*0.5f, half_length = (b-a)*0.5f;
    nodes T(N);

    for (uint k = 0;k<N;k++)
        T[k] = mid + half_length*std::cos(M_PI*(2*k+1)/(2*N));

    return T;
}
