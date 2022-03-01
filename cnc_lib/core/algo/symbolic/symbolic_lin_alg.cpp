#include "symbolic_lin_alg.h"

cnc::symbolic::smat::smat(uint w, uint h) : width(w),height(h)
{
    v.resize(w*h);
}

cnc::symbolic::Expression &cnc::symbolic::smat::operator()(uint i, uint j)
{
    return v[j*width+i];
}

cnc::cmat cnc::symbolic::smat::evaluate(const ValuationSystem &V) const
{
    cmat E(width,height);
    for (uint j = 0;j<height;j++)
        for (uint i = 0;i<width;i++)
            E(i,j) = v[j*width+i].evaluate(V);
    return E;
}

cnc::symbolic::svec::svec(uint n) : h(n)
{
    v.resize(n);
}

cnc::symbolic::smat cnc::symbolic::svec::jacobian(const std::vector<Variable> &X) const
{
    smat J(X.size(),h);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<X.size();i++)
            J(i,j) = v[j].differentiate(X[i]);
    return J;
}

cnc::symbolic::Expression &cnc::symbolic::svec::operator()(uint j)
{
    return v[j];
}

cnc::cvec cnc::symbolic::svec::evaluate(const ValuationSystem &V) const
{
    cvec X(h);
    for (uint j = 0;j<h;j++)
        X(j) = v[j].evaluate(V);
    return X;
}

cnc::vec cnc::symbolic::svec::evaluate_real(const ValuationSystem &V) const
{
    auto cX = evaluate(V);
    vec X(h);
    for (uint i = 0;i<h;i++)
        X(i) = cX(i).real();
    return X;
}

std::string cnc::symbolic::svec::print() const
{
    std::string s;
    for (const auto& x : v)
        s += x.print() + '\n';
    return s;
}
