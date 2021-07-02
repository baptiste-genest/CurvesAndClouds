#ifndef FIBRATION_H
#define FIBRATION_H

#include "geodesy.h"
#include "knot.h"

namespace fibration
{

vec S(scalar theta);
vec CS(scalar theta);

scalar theta();

scalar f(const vec&);
vec grad1(const vec&);
vec grad2(const vec&);
mat grad_space(const vec& x);

vector_function build_pather();
};

#endif // FIBRATION_H
