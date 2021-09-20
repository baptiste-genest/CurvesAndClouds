#ifndef POLYNOMIAL_CURVE_H
#define POLYNOMIAL_CURVE_H

#include "geometry.h"

namespace cnc {

namespace algo {

namespace geometry {

class PolynomialCurve
{
public:
    PolynomialCurve(const vec& p1,const vec& n1,const vec& p2,const vec& n2);

    vec eval(scalar t) const;
private:
    std::vector<vec> coefs;
    uint dim;
    uint degree_plus_one;
};

}

}

}


#endif // POLYNOMIAL_CURVE_H
