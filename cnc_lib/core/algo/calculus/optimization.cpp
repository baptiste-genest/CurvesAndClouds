#include "optimization.h"
using namespace cnc;


scalar cnc::algo::calculus::optimization::newton_method_1D(const cnc::algo::calculus::scalar_function_1D &f, const cnc::algo::calculus::scalar_function_1D &df, scalar x0, scalar eps,uint max_iter)
{
    scalar x = x0,fx;
    for (uint iter = 0;iter<max_iter;iter++){
        fx = f(x);
        if (std::abs(fx) < eps)
            return x;
        x -= fx/df(x);
    }
    throw Cnc_error("Newton method didn't converged");
}

cnc::vec cnc::algo::calculus::optimization::gradient_descent(const cnc::algo::calculus::scalar_function &f, const cnc::vec &x0, scalar eps,uint max_iter)
{
    scalar alpha,dx = eps/10.f;
    auto grad = vector_calculus::build_gradient(f,x0.size(),dx);
    auto grad_norm = [grad] (const vec& x){
        return grad(x).norm();
    };
    vec x = x0,G;scalar gn;
    for (uint iter = 0;iter < max_iter;iter++){
        G = grad(x); gn = grad_norm(x);
        if (gn < eps)
            return x;
        alpha = -min_dichotomia(build_descent_function(grad_norm,x,G*(-1.f)),0.f,std::min(scalar(1),gn),eps,10);
        x += grad(x)*alpha;
    }
    return x;
}

scalar cnc::algo::calculus::optimization::min_dichotomia(const cnc::algo::calculus::scalar_function_1D &f, scalar a, scalar b,scalar eps,uint max_iter)
{
    scalar xa = a,xb = b,m;
    for (uint k = 0;k<max_iter;k++){
        m = (xa + xb)*0.5f;
        if (xb-xa < eps)
            return m;
        if (f(xa)*f(m) < 0)
            xb = m;
        else
            xa = m;
    }
    return m;
}


cnc::algo::calculus::scalar_function_1D cnc::algo::calculus::optimization::build_descent_function(const cnc::algo::calculus::scalar_function &f, const cnc::vec &x, const cnc::vec &d){
    return [f,x,d] (scalar t){
        return f(x+d*t);
    };
}

cnc::vec cnc::algo::calculus::optimization::gradient_descent_fixed_step(const cnc::algo::calculus::scalar_function &f, const cnc::vec &x0, scalar h, scalar eps,scalar dx, uint max_iter)
{
    auto grad = vector_calculus::build_gradient(f,x0.size(),dx);
    return gradient_descent_fixed_step(grad,x0,h,eps,max_iter);
}

vec algo::calculus::optimization::gradient_descent_fixed_step(const algo::calculus::vector_function &grad, const vec &x0, scalar h, scalar eps, uint max_iter)
{
    vec x = x0,G;scalar gn;
    for (uint iter = 0;iter < max_iter;iter++){
        G = grad(x); gn = G.norm();
        std::cout << "grad norm: " << gn << std::endl;
        if (gn < eps){
            return x;
        }
        x -= grad(x)*h;
    }
    return x;

}

vec algo::calculus::optimization::gradient_descent_adaptive_step(const algo::calculus::vector_function &grad, const vec &x0, scalar h, scalar eps, uint max_iter)
{
    vec xprev = x0,x,G = grad(x0),Gprev;
    scalar theta = 10e8,next_h,grad_norm;
    x = x0 - G*h;
    for (uint iter = 0;iter < max_iter;iter++){
        Gprev = G;
        G = grad(x);
        next_h = std::min(std::sqrt(1+theta)*h,x.distance(xprev)/(2*Gprev.distance(G)));
        xprev = x;
        x = x - G*next_h;
        theta = next_h/h;
        h = next_h;
        grad_norm = G.norm_inf();
        std::cout << "grad norm at " << iter << ": " << grad_norm << std::endl;
        if (grad_norm < eps)
            return x;
    }
    return x;
}
