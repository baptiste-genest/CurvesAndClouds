#ifndef SAMPLER_H
#define SAMPLER_H

#include <curvesandcloud.h>
#include "geodesy.h"

using namespace cnc;
using namespace cnc::algo::calculus;

class sampler
{
public:
    sampler(const geodesy& f,scalar dt,uint N);
    cloud sample(const vec& v0);

private:
    geodesy pather;
    scalar dt;
    uint N;
    constexpr static scalar min_dist = 1e-2;
};

#endif // SAMPLER_H
