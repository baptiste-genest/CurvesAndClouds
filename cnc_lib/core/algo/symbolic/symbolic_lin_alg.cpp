#include "symbolic_lin_alg.h"

cnc::symbolic::varSet cnc::symbolic::SymbolicArray::getVariablesInvolved() const
{
    varSet VI;
    for (const auto& E : v)
        for (const auto& var : E.getVariables())
            VI.insert(var);
    return VI;
}

cnc::symbolic::smat::smat(uint w, uint h) : SymbolicArray(w*h),width(w),height(h)
{

}

cnc::mat cnc::symbolic::smat::evaluate(const ValuationSystem &V) const
{
    mat E(height,width);
    for (uint j = 0;j<height;j++)
        for (uint i = 0;i<width;i++)
            E(i,j) = v[j*width+i].evaluate(V).real();
    return E;
}

std::string cnc::symbolic::smat::print() const
{
    std::string out;
    for (uint j = 0;j<height;j++){
        for (uint i = 0;i<width;i++)
            out += v[j*width+i].print() + ' ';
        out += '\n';
    }
    return out;
}

cnc::symbolic::smat cnc::symbolic::smat::transpose() const
{
    smat T(height,width);
    for (uint j = 0;j<height;j++)
        for (uint i = 0;i<width;i++)
            T(j,i) = v[j*width+i];
    return T;
}

cnc::symbolic::smat cnc::symbolic::smat::operator*(const smat &other)
{
    smat P(other.width,height);
    for (uint j = 0;j<P.height;j++)
        for (uint i = 0;i<P.width;i++){
            P(i,j) = Constant::Zero();
            for (uint k = 0;k<width;k++){
                P(i,j) = P(i,j) + (*this)(k,i)*other(j,k);
            }
        }
    return P;
}

cnc::mat cnc::symbolic::smat::operator()(const vec &x) const
{
    mat E(width,height);
    auto VI = getVariablesInvolved();
    if (VI.size() > x.size())
        throw Cnc_error("not all variables involved are valued");
    std::vector<ValuationPair> vp(VI.size());
    uint i = 0;
    for (const auto& v : VI){
        vp[i].first = v;
        vp[i].second = x(i);
        i++;
    }
    return evaluate(ValuationSystem(vp));
}


cnc::symbolic::svec::svec(uint n) : SymbolicArray(n)
{
    v.resize(n);
}

cnc::symbolic::smat cnc::symbolic::svec::jacobian(const std::vector<Variable> &X) const
{
    auto VI = getVariablesInvolved();
    smat J(X.size(),size);
    for (uint j = 0;j<size;j++)
        for (uint i = 0;i<X.size();i++)
            J(i,j) = v[j].differentiate(X[i]);
    return J;
}

cnc::symbolic::smat cnc::symbolic::svec::jacobian() const
{
    auto Vi = getVariablesInvolved();
    smat J(Vi.size(),size);
    for (uint j = 0;j<size;j++){
        uint i = 0;
        for (const auto& var : Vi){
            J(i,j) = v[j].differentiate(Variable::getVariableFromId(var));
            i++;
        }
    }
    return J;
}

cnc::vec cnc::symbolic::svec::evaluate(const ValuationSystem &V) const
{
    vec X(size);
    for (uint i = 0;i<size;i++)
        X(i) = v[i].evaluate(V).real();
    return X;
}

cnc::vec cnc::symbolic::svec::operator()(const cnc::vec &x) const
{
    auto VI = getVariablesInvolved();
    if (VI.size() != x.size())
        throw Cnc_error("not all variables involved are valued");
    std::vector<ValuationPair> vp(VI.size());
    uint i = 0;
    for (const auto& v : VI){
        vp[i].first = v;
        vp[i].second = x(i);
        i++;
    }
    return evaluate(ValuationSystem(vp));
}

std::string cnc::symbolic::svec::print() const
{
    std::string s;
    for (const auto& x : v)
        s += x.print() + '\n';
    return s;
}
