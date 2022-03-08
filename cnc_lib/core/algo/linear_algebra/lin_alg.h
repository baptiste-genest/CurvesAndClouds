/**
  * @file lin_alg.h define types for linear algebra in CNC
  */
#ifndef LIN_ALG_H
#define LIN_ALG_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "../../../cnc_types.h"
#include "../../matrix.h"

namespace cnc {

/**
 * @brief vec N dimensional vector
 */
using vec = chaskal::Vector<scalar>;
using cvec = chaskal::Vector<complex_scalar>;
/**
 * @brief mat NxM matrix
 */
using mat = chaskal::Matrix<scalar>;
using cmat = chaskal::Matrix<complex_scalar>;

namespace linear_utils {
inline vec vec2(scalar x= 0,scalar y=0){
    return vec({x,y});
}
inline vec vec2(const vec& x){
    return vec2(x(0),x(1));
}
inline vec vec3(scalar x,scalar y,scalar z){
    return vec({x,y,z});
}
inline vec vec3(const vec& x){
    return vec3(x(0),x(1),x(2));
}

inline cvec cvec2(scalar x= 0,scalar y=0){
    return cvec({cscalar(x),cscalar(y)});
}

inline vec toPolar(scalar x,scalar y){
    scalar r = sqrt(x*x + y*y);
    scalar th = std::atan2(y,x);
    return vec2(r,th);
}

inline vec toSpherical(scalar x,scalar y,scalar z){
    scalar r = sqrt(x*x + y*y + z*z);
    scalar th = std::acos(z/r);
    scalar phi = std::atan2(y,x);
    return vec3(r,th,phi);
}
inline vec toSpherical(const vec& X){
    return toSpherical(X(0),X(1),X(2));
}

vec ones(uint n);

inline cmat Id(uint n){
    return chaskal::Id<cscalar>(n);
}

namespace eigen {
    std::vector<vec> eigenvector22(const mat& A);
}

}

/*
vec vec2(scalar x,scalar y);
vec vec3(scalar x,scalar y);
*/

}
#endif // LIN_ALG_H
