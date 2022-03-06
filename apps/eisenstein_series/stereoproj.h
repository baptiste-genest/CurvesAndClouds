#ifndef STEREOPROJ_H
#define STEREOPROJ_H

#include "curvesandcloud.h"
using namespace cnc;


vec stereoproj(const cscalar& z1,const cscalar& z2,scalar pole = 0.87);

struct stereopole{
    mat P,Pi;
    stereopole();
    mat getMat(std::array<scalar,2> th) const;
};

vec stereoproj(vec X,scalar th1,scalar th2,scalar pole = 0.87);

vec standard_stereoproj(vec X,scalar pole);

#endif // STEREOPROJ_H
