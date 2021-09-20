#include "polynomial_curve.h"

cnc::algo::geometry::PolynomialCurve::PolynomialCurve(const cnc::vec &p1, const cnc::vec &n1, const cnc::vec &p2, const cnc::vec &n2)
{
    dim = p1.rowNum();
    degree_plus_one = 4;
    coefs.resize(degree_plus_one,vec(dim));
    mat M(degree_plus_one);
    M(3,0) = 1;//p1 condition
    M(1,1) = 2;//n1 condition
    for (uint i = 0;i<degree_plus_one;i++)
        M(i,2) = 1; // p2 condition
    M(0,3) = 6;//n2 condition
    M(1,3) = 2;//n2 condition
    mat I = M.invert();
    for (uint k = 0;k<dim;k++){
        vec C(degree_plus_one);
        C(0) = p1(k);
        C(1) = n1(k);
        C(2) = p2(k);
        C(3) = n2(k);
        C = I*C;
        for (uint i = 0;i<degree_plus_one;i++)
            coefs[degree_plus_one - 1 - i](k) = C(i);
    }
}

cnc::vec cnc::algo::geometry::PolynomialCurve::eval(cnc::scalar t) const
{
    vec R(dim);
    scalar x = 1;
    for (uint k = 0;k<degree_plus_one;k++){
        R += coefs[k]*x;
        x *= t;
    }
    return R;
}
