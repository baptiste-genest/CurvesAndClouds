#include "algo.h"

cnc::range cnc::algo::get_min_max_range(const std::vector<scalar>& X)
{
    range R;
    R.first = *std::min_element(X.begin(),X.end());
    R.second = *std::max_element(X.begin(),X.end());
    return R;
}

std::vector<int> cnc::algo::get_index_lin_space(int a, int b, int dx)
{
    if ((b-a)*dx <= 0)
        throw Cnc_error("b-a must have the same sign than dx");

    const uint N = std::floor(std::abs(b-a)/dx) + 1;

    std::vector<int> X(N);
    X[0] = a;
    for (uint i = 0;i<N-1;i++)
        X[i+1] = X[i]+dx;

    return X;
}

bool cnc::algo::check_unicity(const std::vector<scalar> &v,scalar eps)
{
    for (uint j = 0;j<v.size();j++)
        for (uint i = 0;i<v.size();i++)
            if (i != j && std::abs(v[i]-v[j]) < eps)
                return false;
    return true;
}

void cnc::algo::scale_range(cnc::range &r, scalar s)
{
    scalar offset = (r.second+r.first)*0.5f;
    r.first -= offset;
    r.second -= offset;
    r.first *= s;
    r.second *= s;
    r.first += offset;
    r.second += offset;
}

cnc::range cnc::algo::sort(const cnc::range &r )
{
    return {std::min(r.first,r.second),std::max(r.first,r.second)};
}

int cnc::algo::sign(scalar x)
{
    if (x < 0.f)
        return -1.f;
    return 1.f;
}

cnc::range cnc::algo::inter(const cnc::range &A, const cnc::range &B)
{
    return {std::max(A.first,B.first),std::min(A.second,B.second)};
}

cnc::algo::set_type cnc::algo::inter_type(const cnc::range &A, const cnc::range &B)
{
    scalar x = std::max(A.first,B.first);
    scalar y = std::min(A.second,B.second);
    if (x < y)
        return continuous;
    else if ( std::abs(x-y) < 1e-12)
        return discrete;
    return empty;
}

bool cnc::algo::is_sorted(const cnc::range &r)
{
    return r.first < r.second;
}

std::vector<cnc::range> cnc::algo::get_min_max_range(const std::vector<cnc::vec> &X)
{
    std::vector<range> R(X[0].rowNum());
    std::vector<scalar> coord(X.size());
    for (uint j = 0;j<X[0].rowNum();j++){
        for (uint i = 0;i<X.size();i++)
            coord[i] = X[i](j);
        R[j] = get_min_max_range(coord);
    }
    return R;
}

void cnc::algo::extend_range(cnc::range &A, const cnc::range &B)
{
    A.first = std::min(A.first,B.first);
    A.second = std::max(A.second,B.second);
}
