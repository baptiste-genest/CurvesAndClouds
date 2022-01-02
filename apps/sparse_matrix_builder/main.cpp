#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;

vec ConjugateGradient(const smat& A,const vec& b,const vec& x0,scalar eps){
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
        if (false)
            std::cout <<"conj grad error: " << err << std::endl;
    }
    return x;
}


int main()
{
    uint N = 10;
    SMB H(N,true);
    vec b(N);
    for (uint i = 0;i<N;i++){
        H(i,i) = i+1;
        b(i) = 1;
    }
    smat M(H);
    std::cout << M*b << std::endl;
    std::cout << ConjugateGradient(M,b,vec(N),1e-10) << std::endl;
    //M.print();

    return 0;
}
