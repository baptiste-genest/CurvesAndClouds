#include "dual_numbers.h"

using namespace cnc;
using namespace cnc::dual;

DualNumber DualNumber::operator+(const DualNumber &other) const
{
    order_match(other);
    DualNumber* D = nullptr;
    if (der)
        D = new DualNumber((*der)+(*other.der));
    return DualNumber(val+other.val,D);
}

DualNumber DualNumber::operator-(const DualNumber &other) const
{
    order_match(other);
    DualNumber* D = nullptr;
    if (der)
        D = new DualNumber((*der)-(*other.der));
    return DualNumber(val-other.val,D);
}

DualNumber DualNumber::operator*(const DualNumber &other) const
{
    order_match(other);
    return *mult(this,&other,order);
}

DualNumber* DualNumber::mult(const DualNumber *u, const DualNumber *v, uint length)
{
    if (!length){
        return Number(u->val*v->val);
    }
    return new DualNumber(u->val*v->val,add(mult(u,v->der,length-1),mult(u->der,v,length-1),length-1));
}

DualNumber *DualNumber::add(const DualNumber *u, const DualNumber *v, uint length)
{
    if (!length)
        return Number(u->val+v->val);
    return new DualNumber(u->val+v->val,add(u->der,v->der,length-1));
}

DualNumber DualNumber::operator/(const DualNumber &other) const
{
    /*
    order_match(other);
    return DualNumber(val/other.val,(der*other.val-val*other.der)/(other.val*other.val));
    */
    return DualNumber(1);
}

DualNumber DualNumber::operator*(scalar lambda) const
{
    DualNumber* D = nullptr;
    if (der)
        D = new DualNumber((*der)*lambda);
    return DualNumber(val*lambda,D);
}


/*
DualNumber sin(const DualNumber& x)
{
    return DualNumber(std::sin(x(0)),sin(x.der,x.order));
}

DualNumber cos(const DualNumber& x)
{
    order_match(other);
    return DualNumber(std::sin(x.value()),x.derivative()*std::cos(x.value()));
}

DualNumber exp(const DualNumber &x)
{
    order_match(other);
    return DualNumber(std::exp(x.value()),x.derivative()*std::exp(x.value()));
}

DualNumber pow(const DualNumber &x, scalar n)
{
    order_match(other);
    return DualNumber(std::pow(x.value(),n),x.derivative()*n*std::pow(x.value(),n-1));
}

DualNumber inv(const DualNumber &x)
{
    order_match(other);
    return DualNumber(1./x.value(),-x.derivative()/(x.value()*x.value()));
}

vec value(const dualVec &v)
{
    vec V(v.size());
    for (uint i = 0;i<v.size();i++)
        V(i) = v(i).value();
    return V;
}

vec derivative(const dualVec &v)
{
    vec V(v.size());
    for (uint i = 0;i<v.size();i++)
        V(i) = v(i).derivative();
    return V;
}

void withRespectTo(dualVec &V, uint i)
{
    for (uint k = 0;k<V.size();k++)
        V(k).setDerivative(k==i);
}

*/
