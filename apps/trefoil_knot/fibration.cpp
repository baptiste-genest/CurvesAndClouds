#include "fibration.h"


mat fibration::grad_space(const vec &x)
{
    std::vector<vec> X(3);
    X[0] = grad1(x);
    X[1] = grad2(x);
    X[2] = x;
    return chaskal::build_basis(X);
}

scalar fibration::f(const vec &x)
{
    return std::abs(knot::delta(x)*std::polar(1.,-theta()) - 1.);
}

vec fibration::grad1(const vec &x)
{
    vec g1(4);
    vec f = knot::f(x),s = S(theta());
    vec kg1 = knot::grad1(x);
    vec kg2 = knot::grad2(x);
    vec ki(2);
    for (uint k = 0;k<4;k++){
        ki(0) = kg1(k);
        ki(1) = kg2(k);
        g1(k) = s.scalar_product(ki) - f.scalar_product(ki)/f.norm();
    }
    return g1;
}

vec fibration::grad2(const vec &x)
{
    vec g2(4);
    vec f = knot::f(x),sp = CS(theta());
    vec kg1 = knot::grad1(x);
    vec kg2 = knot::grad2(x);
    vec ki(2);
    for (uint k = 0;k<4;k++){
        ki(0) = kg1(k);
        ki(1) = kg2(k);
        g2(k) = sp.scalar_product(ki);
    }
    return g2;
}

inline vec fibration::S(scalar theta) {
    return vec({cos(theta),sin(theta)});
}

inline vec fibration::CS(scalar theta) {
    return vec({-sin(theta),cos(theta)});
}

inline scalar fibration::theta() {
    return M_PI/3;
}
