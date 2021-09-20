#ifndef POLYNOMIAL_CURVE_H
#define POLYNOMIAL_CURVE_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "geometry.h"

namespace cnc {

namespace algo {

namespace geometry {

class PolynomialCurve
{
public:
    PolynomialCurve(const vec& p1,const vec& n1,const vec& p2,const vec& n2);

    vec eval(scalar t) const;
    inline vec operator()(scalar t) const {
        return eval(t);
    }
private:
    std::vector<vec> coefs;
    uint dim;
    uint degree_plus_one;
};

}

}

}


#endif // POLYNOMIAL_CURVE_H
