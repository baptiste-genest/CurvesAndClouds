#include "calculus.h"
using namespace cnc;


cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::build_1D_linear_interpolator(const std::vector<scalar> &X, const std::vector<scalar> &Y)
{
    if (X.size() != Y.size())
        throw Cnc_error("must have the same number of X and Y coords");
    else if (X.size() < 1)
        throw Cnc_error("must have at least 2 points to interpolate");
    else if (!std::is_sorted(X.begin(),X.end())){
        std::cout << X[0] << " " << X[1] << std::endl;
        throw Cnc_error("X coords must be in ascending order");
    }
    for (uint i = 0;i<X.size()-1;i++){
        if (X[i+1] == X[i])
            throw Cnc_error("X coords must be distincts");
    }
    return [X,Y] (scalar x) {
        if (x < X[0])
            return Y[0];
        if (x > X.back())
            return Y.back();
        for (uint i = 0;i<X.size()-1;i++){
            if (x >= X[i] && x <= X[i+1]){
                scalar l = X[i+1]-X[i];
                scalar t = (x-X[i])/l;
                return Y[i]*(1-t) + Y[i+1]*t;
            }
        }
        return scalar(0);
    };
}

std::vector<scalar> cnc::algo::calculus::get_lin_space(scalar a, scalar b, scalar dx)
{
    if ((b-a)*dx <= 0.f)
        throw Cnc_error("b-a must have the same sign than dx");

    const uint N = std::floor(std::abs(b-a)/dx) + 1;

    std::vector<scalar> X(N);
    X[0] = a;
    for (uint i = 0;i<N-1;i++)
        X[i+1] = X[i]+dx;

    return X;
}

std::vector<scalar> cnc::algo::calculus::get_lin_space(scalar a, scalar b, int N)
{
    if (N <= 0)
        throw Cnc_error("can't build linear space with non positive node number");

    const uint uN = (uint)N;
    const scalar dx = (b-a)/scalar(N-1);

    std::vector<scalar> X(N);
    X[0] = a;
    for (uint i = 0;i<uN-1;i++)
        X[i+1] = X[i]+dx;

    return X;
}

cnc::algo::calculus::nodes cnc::algo::calculus::sample(const cnc::algo::calculus::scalar_function_1D &f, const cnc::algo::calculus::nodes &X)
{
    if (X.size() == 0)
        throw Cnc_error("can't sample a function on an empty node set");
    nodes Y(X.size());
    for (uint i= 0;i<X.size();i++)
        Y[i] = f(X[i]);
    return Y;
}

cnc::algo::calculus::nodes cnc::algo::calculus::build_integration_mesh(const cnc::algo::calculus::scalar_function_1D &f, scalar a, scalar b, scalar dx)
{
    scalar_function_1D fprime = [f,dx] (scalar x){
        return (f(x+dx)-f(x))/dx;
    };
    nodes mesh;
    nodes lin_mesh = get_lin_space(a,b,dx);
    mesh.push_back(a);
    scalar x_n = a;
    while (x_n < b){
        scalar df = fprime(x_n),adf = std::abs(df);
        if (adf < dx){
            bool changed = false;
            for (uint i = 0;i<lin_mesh.size();i++)
                if (lin_mesh[i] > x_n && std::abs(fprime(lin_mesh[i])) > dx){
                    x_n = lin_mesh[i];
                    lin_mesh.erase(lin_mesh.begin(),lin_mesh.begin()+i);
                    changed = true;
                    break;
                }
            if (!changed)
                x_n = b;
        }
        else
            x_n += std::min(dx,1.f/adf);

        mesh.push_back(x_n);
    }
    return mesh;
}

cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::build_range_mapper(const cnc::range &A, const cnc::range &B,bool constrained)
{
    scalar l = std::abs(A.first - A.second);
    if (!constrained)
        return [A,B,l] (scalar x) {
            scalar t = (x-A.first)/l;
            return (1.f-t)*B.first + t*B.second;
        };
    else
        return [A,B,l] (scalar x){
            scalar df = std::abs(A.first - x);
            scalar ds = std::abs(A.second - x);
            if (ds < l && df < l){
                scalar t = std::abs(A.first - x)/l;
                return (1.f-t)*B.first + t*B.second;
            }
            if (df > ds)
                return B.second;
            else
                return B.first;
        };
}
cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::build_ordered_range_mapper(const cnc::range &A, const cnc::range &B,bool constrained)
{
    scalar l = A.second - A.first;
    if (!constrained)
        return [A,B,l] (scalar x) {
            scalar t = (x-A.first)/l;
            return B.first*(1-t) + t*B.second;
        };
    else
        return [A,B,l] (scalar x){
            if (x < A.first)
                return B.first;
            else if (x > A.second)
                return B.second;
            scalar t = (x-A.first)/l;
            return B.first*(1-t) + t*B.second;
        };
}


cnc::algo::calculus::scalar_function_2D cnc::algo::calculus::build_2D_laplacian(const cnc::algo::calculus::scalar_function_2D &f,scalar dx)
{
    if (dx < 0.f)
        throw Cnc_error("can't build laplacian with negativ dx");
    return [f,dx] (scalar x,scalar y) {
        return -(4.f*f(x,y) - f(x+dx,y) - f(x-dx,y) - f(x,y+dx) - f(x,y-dx))/(dx*dx);
    };
}

std::vector<scalar> cnc::algo::calculus::get_lin_space(cnc::range x, scalar dx) {
    return get_lin_space(x.first,x.second,dx);
}

std::vector<scalar> cnc::algo::calculus::get_lin_space(cnc::range x, int N)
{
    return get_lin_space(x.first,x.second,N);
}

cnc::algo::calculus::nodes cnc::algo::calculus::convert_to_nodes(const cnc::vec &x)
{
    if (x.size() == 0)
        throw Cnc_error("can't convert a null vector to node");
    uint n = x.rowNum();
    nodes N(n);
    for (uint k = 0;k<n;k++)
        N[k] = x(k);
    return N;
}

int algo::calculus::get_node_number(const nodes &n, scalar x)
{
    if (n.size() < 2)
        throw Cnc_error("get_node_number takes nodes of size at least 2");
    if (x < n[0])
        return -1;
    if (x > n.back())
        return -2;
    for (uint k = 0;k<n.size()-1;k++)
        if (x > n[k] && x < n[k+1])
            return k;
    throw Cnc_error("error while computing node number");
}

scalar algo::calculus::smallest_positive_quadratic_root(scalar a, scalar b, scalar c)
{
    scalar delta = b*b - 4*a*c;
    if (delta < 0)
        throw Cnc_error("no roots");
    scalar droot = std::sqrt(delta);
    scalar r1 = (-b + droot)/(2*a);
    scalar r2 = (-b - droot)/(2*a);
    if (r1 < 0 && r2 < 0)
        throw Cnc_error("No positive roots");
    if (r1 < 0)
        return r2;
    if (r2 < 0)
        return r1;
    return std::min(r1,r2);
}

std::vector<scalar> algo::calculus::quadratic_roots(scalar a, scalar b, scalar c)
{
    scalar delta = b*b - 4*a*c;
    if (delta < 0)
        return {};
    scalar droot = std::sqrt(delta);
    scalar r1 = (-b + droot)/(2*a);
    scalar r2 = (-b - droot)/(2*a);
    return {r1,r2};
}

