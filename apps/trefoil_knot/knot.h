#ifndef KNOT_H
#define KNOT_H

#include <curvesandcloud.h>
using namespace cnc;
using namespace cnc::algo::calculus;

namespace knot {

const scalar sc = -27;

vec f(const vec&);
vec grad1(const vec&);
vec grad2(const vec&);
mat grad_space(const vec& x);

complex_scalar delta(const vec& x);

}

#endif // KNOT_H
