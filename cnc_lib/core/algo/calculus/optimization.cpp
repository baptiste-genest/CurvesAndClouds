#include "optimization.h"


float cnc::algo::calculus::optimization::newton_method_1D(const cnc::algo::calculus::scalar_function_1D &f, const cnc::algo::calculus::scalar_function_1D &df, float x0, float eps,uint max_iter)
{
    float x = x0,fx;
    for (uint iter = 0;iter<max_iter;iter++){
        fx = f(x);
        if (std::abs(fx) < eps)
            return x;
        x -= fx/df(x);
    }
    throw Cnc_error("Newton method didn't converged");
}

cnc::vec cnc::algo::calculus::optimization::gradient_descent(const cnc::algo::calculus::scalar_function &f, const cnc::vec &x0, float eps,uint max_iter)
{
    float alpha,dx = eps/10.f;
    auto grad = vector_calculus::build_gradient(f,x0.size(),dx);
    auto grad_norm = [grad] (const vec& x){
        return grad(x).norm();
    };
    vec x = x0,G;float gn;
    for (uint iter = 0;iter < max_iter;iter++){
        G = grad(x); gn = grad_norm(x);
        if (gn < eps)
            return x;
        alpha = -min_dichotomia(build_descent_function(grad_norm,x,G*(-1.f)),0.f,std::min(1.f,gn),eps,10);
        x += grad(x)*alpha;
    }
    return x;
}

float cnc::algo::calculus::optimization::min_dichotomia(const cnc::algo::calculus::scalar_function_1D &f, float a, float b,float eps,uint max_iter)
{
    float xa = a,xb = b,m;
    for (uint k = 0;k<max_iter;k++){
        m = (xa + xb)*0.5f;
        if (std::abs(f(m)) < eps)
            return m;
        if (std::abs(f(xa)) < std::abs(f(m)))
            xb = m;
        else
            xa = m;
    }
    return m;
}


cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::optimization::build_descent_function(const cnc::algo::calculus::scalar_function &f, const cnc::vec &x, const cnc::vec &d){
    return [f,x,d] (float t){
        return f(x+d*t);
    };
}

cnc::vec cnc::algo::calculus::optimization::gradient_descent_fixed_step(const cnc::algo::calculus::scalar_function &f, const cnc::vec &x0, float h, float eps, uint max_iter)
{
    float dx = eps/10.f;
    auto grad = vector_calculus::build_gradient(f,x0.size(),dx);
    h *= -1.f;
    vec x = x0,G;float gn;
    for (uint iter = 0;iter < max_iter;iter++){
        G = grad(x); gn = G.norm();
        if (gn < eps){
            return x;
        }
        x += grad(x)*h;
    }
    return x;
}
