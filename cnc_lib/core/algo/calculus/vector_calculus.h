/**
  * @file vector_calculus.h namespace that defines algorithms and data struct to handle vector calculus
  */
#ifndef VECTOR_CALCULUS_H
#define VECTOR_CALCULUS_H

#include "calculus.h"
#include "../lin_alg.h"

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

namespace cnc {

namespace algo {

/**
 * @brief namespace that stores algorithms and data struct to handle vector calculus
 */
namespace vector_calculus {

/**
 * @brief The Vector_2D_array class allows to store and compute discrete sampled 2D vector field (for fluid simulation for instance)
 */
class Vector_2D_array{
public:
    /**
     * @brief Vector_2D_array initialize a null vector field of size W*H
     * @param W number of vector by width
     * @param H number of vector by height
     */
    inline Vector_2D_array(uint W,uint H) : w(W), h(H){	value.resize(W*H,vec(2));	}
    /**
     * @brief Vector_2D_array initialize a null vector field of size N*N
     * @param N number of vector by side
     */
    inline Vector_2D_array(uint N) : w(N),h(N) {	value.resize(N*N,cnc::vec(2));	}

    /**
     * @brief Vector_2D_array initialize a vector field of size (X.size)*(Y.size) with value of V computes at nodes
     * @param X nodes of x coords
     * @param Y nodes of y coords
     * @param V function to sample
     */
    Vector_2D_array(const calculus::nodes& X,const calculus::nodes& Y,const calculus::vector_function_2D& V);

    /**
     * @brief operator () access to vector at [i,j]
     * @param i width coordinate
     * @param j height coordinate
     * @return const ref to the vector
     */
    const vec& operator()(uint i,uint j) const{
        return value[j*w + i];
    }

    /**
     * @brief operator () access to modify the vector at [i,j]
     * @param i width coordinate
     * @param j height coordinate
     * @return vector address
     */
    vec& operator()(uint i,uint j){
        return value[j*w + i];
    }

    /**
     * @brief get_interpolator build vector interpolator in given ranges, use bilinear intepolation
     * @param x real range to map from x to width (if x is [0,1] 0 will be the the first col and 1 the last)
     * @param y real range to map from y to height (if y is [0,1] 0 will be the the first row and 1 the last)
     * @return interpolator
     */
    calculus::vector_function_2D get_interpolator(const range& x,const range& y) const ;

    /**
     * @brief operator + add two vector fields of same sizes
     * @param O VF to add
     * @return sum
     */
    Vector_2D_array operator+(const Vector_2D_array& O) const ;
    /**
     * @brief operator + substract two vector fields of same sizes
     * @param O VF to sub
     * @return sub
     */
    Vector_2D_array operator-(const Vector_2D_array& O) const ;
    /**
     * @brief operator * return scaled vector field
     * @param l scalar parameters
     * @return scaled vector field
     */
    Vector_2D_array operator*(scalar l) const ;


    /**
     * @brief get_helmoltz_hodge_decomposition return helmoltz hodge decomposition WIP (only works on small grid)
     * @return [U,V] with this VF = U + V and div(U) = 0 and rot(V) = 0
     */
    std::pair<Vector_2D_array,Vector_2D_array> get_helmoltz_hodge_decomposition() const;

private:
    vec ix(uint i,uint j) const{
        return value[j*w + i];
    }

    std::vector<vec> value;
    uint w;
    uint h;

    vec get_vec_form_divergence() const;

    static Vector_2D_array build_gradient_from_matrix(const mat& M);
};

/**
 * @brief build_gradient build gradient application of a 2D scalar field
 * @param f scalar field
 * @param dx space step
 * @return gradient function (x,y) -> Grad(f)(x,y) (vector valued)
 */
calculus::vector_function_2D build_gradient(const calculus::scalar_function_2D& f,scalar dx = 0.001f);

/**
 * @brief build_gradient build gradient application of a n-D scalar field
 * @param f scalar field
 * @param dx space step
 * @return gradient function (x) -> Grad(f)(x) (vector valued)
 */
calculus::vector_function build_gradient(const calculus::scalar_function& f,uint dim,scalar dx = 1e-4);
/**
 * @brief build_divergence build divergence application of a 2D vector field
 * @param f vector field
 * @param dx space step
 * @return divergence function (x,y) -> div(f)(x,y) (scalar valued)
 */
calculus::scalar_function_2D build_divergence(const calculus::vector_function_2D& f,scalar dx = 0.001f);
/**
 * @brief build_divergence build rot norm application of a 2D vector field
 * @param f vector field
 * @param dx space step
 * @return ||rot(F)|| function (x,y) -> sign(rot(f)(x,y)_z)*||rot(f)(x,y)|| (scalar valued)
 */
calculus::scalar_function_2D build_rot_norm(const calculus::vector_function_2D& f,scalar dx = 0.001f);

}

}

}



#endif // VECTOR_CALCULUS_H
