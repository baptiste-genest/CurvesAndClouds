/**
  * @file differential_equations.h file defining solvers for ODE/ (1D) PDE
  */
#ifndef DIFFERENTIAL_EQUATIONS_H
#define DIFFERENTIAL_EQUATIONS_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
#include "calculus.h"
#include "../statistics/statistics.h"

namespace cnc {

namespace algo {

/**
 * @brief namespace containing algorithms to solve ODE/PDE
 */
namespace differential_equations {


/**
 * @brief ODE solving functions and numerical schemes
 */
namespace ODE {
/**
 * @brief ODE_steps type containing all the steps computed during ODE solving
 */
typedef std::vector<std::pair<vec,scalar>> ODE_steps;
/**
 * @brief extract_space_steps remove time information from an ODE_steps to only keep position (usually for plotting^)
 * @param S ODE steps to select from
 * @return std::vector of all the positions computed
 */
std::vector<vec> extract_space_steps(const ODE_steps& S);

/**
 * @brief differential derivative (here f) in the associated cauchy problem u'(t) = f(u(t),t)
 */
typedef std::function<vec(const vec&,scalar)> differential;
/**
 * @brief ODE_scheme numerical scheme to solve the ODE, must give u_{n+1} given u_n and t
 */
typedef std::function<vec(const vec&,scalar)> ODE_scheme;

/**
 * @brief build_euler_explicit build the ODE_scheme associated with the euler explicit method (order 1)
 * @param f differential of the ODE
 * @param dt time step
 * @return ODE_scheme
 */
ODE_scheme build_euler_explicit(const differential& f,scalar dt);
/**
 * @brief build_runge_kutta_2 build the ODE_scheme associated with the runge kutta 2 method (order 2)
 * @param f differential of the ODE
 * @param dt time step
 * @return ODE_scheme
 */
ODE_scheme build_runge_kutta_2(const differential& f,scalar dt);
/**
 * @brief build_runge_kutta_4 build the ODE_scheme associated with the runge kutta 4 method (order 4)
 * @param f differential of the ODE
 * @param dt time step
 * @return ODE_scheme
 */
ODE_scheme build_runge_kutta_4(const differential& f,scalar dt);

/**
 * @brief solve_ODE computes the approximation of the solution of the ODE, given a scheme, a starting point and time, time step and number of iterations
 * @param s scheme to solve the ODE
 * @param x0 starting point
 * @param t0 start time
 * @param dt time step
 * @param N number of steps
 * @return ODE_steps computed
 */
ODE_steps solve_ODE(const ODE_scheme& s,const vec& x0,scalar t0,scalar dt,uint N);


}

/**
 * @brief PDE solving functions and numerical schemes
 */
namespace PDE {


/**
 * 	@brief PDE_scheme finite differences scheme for PDE of the form d_t u = A(u) + F, where A is a linear tri_diagonal differential operator in space (like the laplacian for instance)
 */
typedef std::function<vec(const vec&)> PDE_scheme;
typedef std::vector<vec> PDE_steps;

/**
 * @brief The boundary_condition_type enum lists possible boundary conditions types of a PDE problem
 * for the moment:
 * 			dirichlet : values at boundarie fixed
 * 			periodic : values at boundaries can change but remains equal
 */
enum boundary_condition_type {
    dirichlet,
    periodic,
    any
};

/**
 * @brief PDE solving in 1D
 */
namespace D1 {

/**
 * @brief build_initial_solution build U0 in vector form given nodes where evaluate a scalar function
 * @param X nodes where evaluate f (U0_i = f(X[i])
 * @param f function to sample
 * @return U0 in vec form
 */
vec build_initial_solution(const calculus::nodes& X,const calculus::scalar_function_1D& f);

/**
 * @brief build_1D_laplacian_operator build finite difference 1D approximation of the space laplacian operator
 * @param n dim of the mesh
 * @param dx space step
 * @param bct boundary condition type can be : dirichlet (boundaries fixed), periodic, or nothing
 * @return
 */
mat build_1D_laplacian_operator(uint n,scalar dx,boundary_condition_type bct = any);
/**
 * @brief build_1D_advection_operator build finite difference 1D approximation of the space advection operator
 * @param n dim of the mesh
 * @param dx space step
 * @param bct boundary condition type can be : dirichlet (boundaries fixed), periodic, or nothing
 * @return
 */
mat build_1D_advection_operator(uint n,scalar dx,boundary_condition_type = any);

/**
 * @brief build_euler_explicit PDE scheme U_n+1 = U_n + dt*(A(u_n) + F) order 1
 * @param A linear space differential operator
 * @param F source term
 * @param dt time step
 * @param bc boundary condition type can be : dirichlet (boundaries fixed), periodic, or nothing
 * @return PDE scheme
 */
PDE_scheme build_euler_explicit(const mat& A,const vec& F,scalar dt,boundary_condition_type bc);
/**
 * @brief build_euler_implicit PDE scheme U_n+1 = U_n + dt*(A(u_n+1) + F) order 1
 * @param A linear space differential operator
 * @param F source term
 * @param dt time step
 * @param bc boundary condition type can be : dirichlet (boundaries fixed), periodic, or nothing
 * @return PDE scheme
 */
PDE_scheme build_euler_implicit(const mat& A,const vec& F,scalar dt,boundary_condition_type bc);
/**
 * @brief build_crank_nicholsan PDE scheme U_n+1 = U_n + 1/2*dt*(A(u_n) + A(u_n+1) + 2*F) order 2
 * @param A linear space differential operator
 * @param F source term
 * @param dt time step
 * @param bc boundary condition type can be : dirichlet (boundaries fixed), periodic, or nothing
 * @return PDE scheme
 */
PDE_scheme build_crank_nicholson(const mat& A,const vec& F,scalar dt,boundary_condition_type bc);


}

/**
 * @brief solve_PDE computes the approximation of the solution of the PDE, given a scheme, a starting mesh and number of iterations
 * @param s scheme to solve the PDE
 * @param u0 starting point
 * @param N number of steps
 * @return PDE_steps computed
 */
PDE_steps solve_PDE(const PDE_scheme& s,const vec& u0,uint N);

}

}

}

}

#endif // DIFFERENTIAL_EQUATIONS_H
