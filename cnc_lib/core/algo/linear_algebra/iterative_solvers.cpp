#include "iterative_solvers.h"

cnc::vec cnc::algo::IterativeSolvers::ConjugateGradient(const smat& A,const vec& b,const vec& x0,scalar eps,bool debug){
    vec r = b - A*x0,p=r,x = x0,Ap,pr;
    scalar alpha,beta;
    scalar err = 2*eps;
    scalar ibm = 1./b.norm_inf();
    int iter = 0;
    while(err > eps && iter < 100){
        iter++;
        if (err > 1e8)
            throw Cnc_error("Conjugate gradient did not converge");
        Ap = A*p;
        alpha = r.norm2() / (p.scalar_product(Ap));
        x = x + p*alpha;
        pr = r;
        r = r - Ap*alpha;
        beta = r.norm2()/(pr.norm2());
        p = r + p*beta;
        err = ((vec)(A*x - b)).norm_inf()*ibm;
        if (debug)
            std::cout <<"conj grad error: " << err << std::endl;
    }
    return x;
}


cnc::vec cnc::algo::IterativeSolvers::ConjugateGradient(const cnc::mat &A, const cnc::vec &b, const cnc::vec &x0, cnc::scalar eps, bool debug)
{
    vec r = b - A*x0,p=r,x = x0,Ap,pr;
    scalar alpha,beta;
    scalar err = 2*eps;
    scalar ibm = 1./b.norm_inf();
    int iter = 0;
    while(err > eps && iter < 100){
        iter++;
        if (err > 1e8)
            throw Cnc_error("Conjugate gradient did not converge");
        Ap = A*p;
        alpha = r.norm2() / (p.scalar_product(Ap));
        x = x + p*alpha;
        pr = r;
        r = r - Ap*alpha;
        beta = r.norm2()/(pr.norm2());
        p = r + p*beta;
        err = ((vec)(A*x - b)).norm_inf()*ibm;
        if (debug)
            std::cout <<"conj grad error: " << err << std::endl;
    }
    return x;
}


cnc::algo::eigen_mode cnc::algo::IterativeSolvers::InversePowerMethod(const cnc::mat &A, cnc::scalar eps,bool debug)
{
    uint n = A.rowNum();
    vec x = algo::stat::random_var::random_vec(-1,1,n).normalize(),old;
    scalar err = 2*eps;
    while (err > eps){
        old = x;
        x = ConjugateGradient(A,x,x,eps).normalize();
        err = old.distance(x);
        if (debug)
            std::cout << err << std::endl;
    }
    return {(A*x).norm(),x};
}

cnc::algo::eigen_mode cnc::algo::IterativeSolvers::PowerMethod(const cnc::mat &A, cnc::scalar eps, bool debug)
{
    return PowerMethod(A,algo::stat::random_var::random_vec(-1,1,A.rowNum()).normalize(),eps,debug);
}

cnc::algo::eigen_mode cnc::algo::IterativeSolvers::PowerMethod(const cnc::mat &A, const cnc::vec &x0, cnc::scalar eps, bool debug)
{
    vec x = x0,old;
    scalar err = 2*eps;
    while (err > eps){
        old = x;
        x = ((vec)(A*x)).normalize();
        err = old.distance(x);
        if (debug)
            std::cout << err << std::endl;
    }
    return {(A*x).norm(),x};
}

cnc::vec cnc::algo::IterativeSolvers::PreconditionnedConjugateGradient(const cnc::smat &A, const cnc::vec &b, const cnc::MatrixMultiplicationChain &C, const cnc::vec &x0, cnc::scalar eps, bool debug)
{
    vec r = b - A*x0,p=ChainSolve(C,r),x = x0,Ap,pr,z = p,pz;
    scalar alpha,beta;
    scalar err = 2*eps;
    scalar ibm = 1./b.norm_inf();
    int iter = 0;
    while(err > eps && iter < 100){
        iter++;
        if (err > 1e8)
            throw Cnc_error("Conjugate gradient did not converge");
        Ap = A*p;
        alpha = z.scalar_product(r) / (p.scalar_product(Ap));
        x = x + p*alpha;
        pr = r;
        r = r - Ap*alpha;
        pz = z;
        z = ChainSolve(C,r);
        beta = z.scalar_product(r)/pr.scalar_product(pz);
        p = z + p*beta;
        err = ((vec)(A*x - b)).norm_inf()*ibm;
        if (debug)
            std::cout <<"conj grad error: " << err << std::endl;
    }
    return x;
}

