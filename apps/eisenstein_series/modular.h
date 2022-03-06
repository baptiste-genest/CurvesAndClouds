#ifndef MODULAR_H
#define MODULAR_H

#include "curvesandcloud.h"
using namespace cnc;
using namespace cnc::symbolic;

cscalar eta(const cscalar& tau){
    static cscalar I(0,1);
    cscalar E = std::exp((scalar)M_PI/12.*I*tau);
    for (uint i = 1;i<=8;i++){
        E *= cscalar(1.)-std::exp((scalar)M_PI*2.*i*I*tau);
    }
    return E;
}

cscalar eta_eisenstein(const cscalar& w1,const cscalar& w2){
    const static scalar f = std::pow(2*M_PI,12);
    return f*std::pow(w1,-12)*std::pow(eta(w2/w1),24);
}


cscalar lambert(const cscalar& q,uint a){
    if (std::abs(q) > 1.)
        std::cerr << "won't converge" << std::endl;

    cscalar sum = 0;
    auto qn = q;
    static constexpr uint N = 5000;
    for (uint n = 1;n<N;n++){
        sum += std::pow<scalar>(n,a)*qn/((cscalar)1.-qn);
        qn = qn*q;
    }
    return sum;
}

scalar f(scalar x,scalar a,scalar b){
    return x*x*a + x*x*x*b -1.;
}
scalar df(scalar x,scalar a,scalar b){
    return 2.*x*a + 3.*x*x*b;
}

scalar newton(scalar a,scalar b){
    static const scalar eps = 5e-16;

    scalar x = 1.;
    while (std::abs(f(x,a,b)) > eps)
        x -= f(x,a,b)/df(x,a,b);
    if (x < 0)
        throw Cnc_error("neg root");
    return std::pow(x,-0.25);
}

cscalar g2(const cscalar& tau){
    static const scalar factor = 60.*std::pow(M_PI,4)/45;
    cscalar q = std::exp(cscalar(0.,2.*M_PI)*tau);
    return factor*(scalar(1.)+scalar(240.)*lambert(q,3));
}

cscalar g3(const cscalar& tau){
    static const scalar factor = 140.*2*std::pow(M_PI,6)/945;
    cscalar q = std::exp(cscalar(0,2*M_PI)*tau);
    return factor*(scalar(1.)-scalar(504.)*lambert(q,5));
}

mat genMat(uint length){
    mat U(2,2,{1,1,0,1.});
    mat V(2,2,{1,0,1,1.});
    mat A(2,2,{1,0,0,1.});
    std::string prod;
    for (uint n = 0;n<length;n++)
        if (rand()%3 == 0){
            prod += "U";
            A = A * U;
        }
        else {
            prod += "V";
            A = A*V;

        }
    std::cout << "MAT GENERATED: " << prod << std::endl;
    return A;
}

Variable Z[4];
ValuationSystem ValuateVector(const vec& z){
    ValuationSystem V = {Z[0] == z(0),
                         Z[1] == z(1),
                         Z[2] == z(2),
                         Z[3] == z(3)};
    return V;
}

vec ctov(const cscalar& z1,const cscalar& z2){
    return vec({z1.real(),z1.imag(),z2.real(),z2.imag()});
}
cloud generate_trefoil_knot()
{
    cloud trefoil;
    cscalar I(0.,1.);
    const auto& a = Z[0];
    const auto& b = Z[1];
    const auto& c = Z[2];
    const auto& d = Z[3];
    Expression knot = pow(a+b*I,3)-27.*pow(c+d*I,2);
    //Expression knot = pow(a+b*I,3)+pow(c+d*I,2);

    svec G(4);
    for (uint j = 0;j<4;j++)
        G(j) = knot.differentiate(Z[j]);

    svec grads[3] = {svec(4),svec(4),svec(4)};
    for (uint j= 0;j<4;j++){
        grads[0](j) = Re(G(j));
        grads[1](j) = Im(G(j));
        grads[2](j) = Z[j];
    }
    auto X = ctov(std::polar(scalar(1.),scalar(M_PI/3)),I*std::pow(scalar(27.),-0.5));
    //auto X = ctov(1.,I).normalize();
    std::cout << knot.evaluate(ValuateVector(X)) << ' ' << X.norm() << std::endl;
    scalar dt = 5e-4;
    trefoil.add_point(X);
    std::vector<vec> grad_eval(4);
    auto EX = ValuateVector(X);
    for (uint j = 0;j<3;j++)
        grad_eval[j] = grads[j].evaluate_real(EX);
    grad_eval[3] = algo::stat::random_var::random_vec_in_sphere(1.,4);
        //std::cout << P(X) << ' ' << X.norm() << std::endl;
    X += chaskal::gram_schmidt(grad_eval).back()*dt;
    trefoil.add_point(X);
    for (uint i = 0;i<50000;i++){
        EX = ValuateVector(X);
        for (uint j = 0;j<3;j++)
            grad_eval[j] = grads[j].evaluate_real(EX);
        grad_eval[3] = trefoil[i+1]-trefoil[i];
        X += chaskal::gram_schmidt(grad_eval).back()*dt;
        //std::cout << P(X) << ' ' << X.norm() << std::endl;
        trefoil.add_point(X);
        if (X.distance(trefoil[0]) < 0.01 && i > 1000)
            break;
    }
    std::cout << knot.evaluate(ValuateVector(X)) << ' ' << X.norm() << std::endl;
    return trefoil;
}


#endif // MODULAR_H
