#include "stereoproj.h"

stereopole::stereopole() {
    std::vector<vec> vb = algo::stat::random_var::sample_vector_on_unit_sphere(4,4);
    vb = chaskal::gram_schmidt(vb);
    P = chaskal::build_basis(vb);
    Pi = P.transpose();
}

mat stereopole::getMat(std::array<scalar, 2> th) const {
    mat R(4);
    for (uint j = 0;j<2;j++){
        R(2*j,2*j) = cos(th[j]);
        R(2*j+1,2*j) = -sin(th[j]);
        R(2*j,2*j+1) = sin(th[j]);
        R(2*j+1,2*j+1) = cos(th[j]);
    }
    return P*R*Pi;
}

vec stereoproj(const cscalar &z1, const cscalar &z2, scalar pole){
    auto f = 1./(pole-z2.imag());
    return linear_utils::vec3(z1.real()*f,z1.imag()*f,z2.real()*f);
}

vec stereoproj(vec X, scalar th1, scalar th2, scalar pole){
    static stereopole SP;
    X = SP.getMat({th1,th2})*X;
    auto f = 1./(X(3)-pole);
    return linear_utils::vec3(X)*f;
}

vec standard_stereoproj(vec X, scalar pole){
    cscalar w(X(2),X(3));
    w *= cscalar(0.,sqrt(27));
    X(2) = w.real();
    X(3) = w.imag();
    scalar f = 1./(pole-X(3));
    return linear_utils::vec3(X)*f;
}
