#include "knot.h"

vec knot::f(const vec& x){//w^3 + sc*z^2
    const scalar& a = x(0);
    const scalar& b = x(1);
    const scalar& c = x(2);
    const scalar& d = x(3);
    return vec({
                a*a*a - 3*a*b*b + sc*(c*c - d*d),
                3*a*a*b - b*b*b + sc*(2*c*d)
               });
}

vec knot::grad1(const vec& x){
    const scalar& a = x(0);
    const scalar& b = x(1);
    const scalar& c = x(2);
    const scalar& d = x(3);
    return vec({
               3*a*a - 3*b*b,
               -6*a*b,
               2*sc*c,
               -2*sc*d
           });
}

vec knot::grad2(const vec& x){
    const scalar& a = x(0);
    const scalar& b = x(1);
    const scalar& c = x(2);
    const scalar& d = x(3);
    return vec({
               6*a*b,
               3*a*a - 3*b*b,
               2*sc*d,
               2*sc*c
           });
}

mat knot::grad_space(const vec& x){
    std::vector<vec> X(3);
    X[0] = grad1(x);
    X[1] = grad2(x);
    X[2] = x;
    return chaskal::build_basis(X);
}

complex_scalar knot::delta(const vec& X){
    complex_scalar z(X(0),X(1));
    complex_scalar w(X(2),X(3));
    return z*z*z + sc*w*w;
}
