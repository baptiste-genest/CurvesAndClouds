#include "differential_equations.h"
using namespace cnc::algo::differential_equations;

ODE::ODE_scheme ODE::build_euler_explicit(const ODE::differential &f,float dt)
{
    return [f,dt] (const vec& x,float t){
        return x + f(x,t)*dt;
    };
}

ODE::ODE_scheme ODE::build_runge_kutta_2(const ODE::differential &f,float dt)
{
    return [f,dt] (const vec& x,float t){
        vec k1 = f(x,t)*dt;
        vec k2 = f(x+k1,t+dt)*dt;
        return x + (k1 + k2)*0.5f;
    };
}

ODE::ODE_scheme ODE::build_runge_kutta_4(const ODE::differential &f,float dt)
{
    return [f,dt] (const vec& x,float t){
        vec k1 = f(x,t)*dt;
        vec k2 = f(x+k1*0.5f,t + dt*0.5f)*dt;
        vec k3 = f(x+k2*0.5f,t + dt*0.5f)*dt;
        vec k4 = f(x+k3,t + dt)*dt;
        return x + (k1 + k2*2.f + k3*2.f + k4)*(1.f/6.f);
    };
}

ODE::ODE_steps ODE::solve_ODE(const ODE_scheme& s, const cnc::vec &x0, float t0, float dt, uint N)
{
    ODE_steps S(N);
    float t = t0;
    S[0] = {x0,t0};
    for (uint k = 1;k<N;k++){
        t += dt;
        S[k] = {s(S[k-1].first,t),t};
    }
    return S;
}

std::vector<cnc::vec> ODE::extract_space_steps(const ODE::ODE_steps &S)
{
    std::vector<cnc::vec> C(S.size());
    for (uint k = 0;k<S.size();k++)
        C[k] = S[k].first;
    return C;
}

cnc::mat PDE::D1::build_1D_laplacian_operator(uint n,float dx,boundary_condition_type bct)
{
    cnc::mat L(n,n);
    float idx2 = 1.f/(dx*dx);
    for (uint k = 0;k<n;k++){
        L(k,k) = 2.f*idx2;
        if (k > 0)
            L(k-1,k) = -idx2;
        if (k < n-1)
            L(k+1,k) = -idx2;
    }
    if (bct == dirichlet){
        L(0,0) = 0.f;L(1,0) = 0.f;
        L(n-2,n-1) = 0.f;L(n-1,n-1) = 0.f;
    }
    if (bct == periodic){
        L(n-1,0) = -idx2;
        L(0,n-1) = -idx2;
    }
    return L;
}

PDE::PDE_scheme PDE::D1::build_euler_explicit(const cnc::mat &A, const cnc::vec &F, float dt,boundary_condition_type)
{
    uint n = A.rowNum();
    mat EE = chaskal::Id<float>(n) + A*dt;
    return [EE,F,dt] (const vec& u ){
        return EE*u + F*dt;
    };
}

PDE::PDE_scheme PDE::D1::build_euler_implicit(const cnc::mat &A, const cnc::vec &F, float dt,boundary_condition_type bc)
{
    uint n = A.rowNum();
    mat EI = chaskal::Id<float>(n) - A*dt;
    return [EI,F,dt,bc] (const vec& u ){
        if (bc == periodic)
            return EI.tri_diagonal_periodic_solve(u + F*dt);
        else
            return EI.tri_diagonal_solve(u + F*dt);
    };
}

PDE::PDE_scheme PDE::D1::build_crank_nicholson(const cnc::mat &A, const cnc::vec &F, float dt,boundary_condition_type bc)
{
    uint n = A.rowNum();
    mat EI = chaskal::Id<float>(n) - A*dt*0.5;
    mat EE = chaskal::Id<float>(n) + A*dt*0.5;
    return [EI,EE,F,dt,bc] (const vec& u ){
        if (bc == periodic)
            return EI.tri_diagonal_periodic_solve(EE*u+F*dt);
        else
            return EI.tri_diagonal_solve(EE*u+F*dt);
    };
}

PDE::PDE_steps PDE::solve_PDE(const PDE::PDE_scheme &s, const cnc::vec &u0, uint N)
{
    PDE_steps S(N);
    S[0] = u0;
    for (uint k = 1;k<N;k++)
        S[k] = s(S[k-1]);
    return S;
}


cnc::vec PDE::D1::build_initial_solution(const cnc::algo::calculus::nodes &X, const cnc::algo::calculus::scalar_function_1D &f)
{
    uint n = X.size();
    vec U0(n);
    for (uint k = 0;k<n;k++)
        U0(k) = f(X[k]);
    return U0;
}


cnc::mat PDE::D1::build_1D_advection_operator(uint n, float dx, boundary_condition_type bct)
{
    cnc::mat L(n,n);
    float idx = 1.f/dx;
    for (uint k = 0;k<n;k++){
        L(k,k) = -idx;
        if (k < n-1)
            L(k+1,k) = idx;
    }
    if (bct == dirichlet){
        L(0,0) = 0.f;L(1,0) = 0.f;
        L(n-2,n-1) = 0.f;L(n-1,n-1) = 0.f;
    }
    if (bct == periodic)
        L(0,n-1) = idx;
    return L;
}
