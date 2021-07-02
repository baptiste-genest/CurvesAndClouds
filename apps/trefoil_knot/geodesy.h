#ifndef GEODESY_H
#define GEODESY_H

#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo::calculus;

typedef std::function<mat(const vec&)> mapper;
typedef std::function<vec(const vec&,const vec&)> geodesy;

geodesy build_pather(const mapper& M);

#endif // GEODESY_H
