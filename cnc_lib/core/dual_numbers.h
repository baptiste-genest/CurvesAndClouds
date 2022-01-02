#ifndef DUAL_NUMBERS_H
#define DUAL_NUMBERS_H

#include <cnc_types.h>
#include "core/algo/linear_algebra/lin_alg.h"
#include <functional>
#include "cnc_error.h"

namespace cnc {

namespace dual {

class DualNumber
{
public:
    DualNumber(uint n){
        val = 0;
        order = n;
        if (n)
            der = new DualNumber(n-1);
    }
    DualNumber(scalar v,DualNumber* d){
        val = v;
        der = d;
        if (d)
            order = 1+d->order;
        else
            order = 1;
    }
    ~DualNumber(){
        if (der)
            delete der;
    }
    DualNumber operator+(const DualNumber& other) const;
    DualNumber operator*(const DualNumber& other) const;
    DualNumber operator-(const DualNumber& other) const;
    DualNumber operator/(const DualNumber& other) const;
    DualNumber operator*(scalar lambda) const;

    inline scalar operator()(uint i) const {
        if (i == 0)
            return val;
        if (der == nullptr)
            throw Cnc_error("Try to access a derivative higher than fixed order");
        return der->operator()(i-1);
    }
    inline scalar& operator()(uint i) {
        if (i == 0)
            return val;
        if (der == nullptr)
            throw Cnc_error("Try to access a derivative higher than fixed order");
        return der->operator()(i-1);
    }
    inline void setValue(scalar v) {
        val = v;
    }
    friend DualNumber sin(const DualNumber& x);
    friend DualNumber cos(const DualNumber& x);
    friend DualNumber exp(const DualNumber& x);
    friend DualNumber pow(const DualNumber& x,scalar n);
    friend DualNumber inv(const DualNumber& x);

private:
    void order_match(const DualNumber& other) const {
        if (order != other.order)
            throw Cnc_error("Can't operate dual numbers of different orders");
    }
    uint order = 0;
    scalar val;
    DualNumber* der=nullptr;

    static DualNumber* mult(const DualNumber* u,const DualNumber* v,uint length);
    static DualNumber* add(const DualNumber* u,const DualNumber* v,uint length);
    static DualNumber* sin(const DualNumber* u,uint length);
    static DualNumber* cos(const DualNumber* u,uint length);

    static inline DualNumber* Number(scalar x){
        DualNumber* D = new DualNumber(0);
        D->val = x;
        return D;
    }
};


using dualVec = chaskal::Vector<DualNumber>;
using dualMat = chaskal::Matrix<DualNumber>;
using dualFunc = std::function<dualVec(const dualVec&)>;
using dualVecFunc = std::function<dualVec(const dualVec&)>;

dualVec gradient(const dualFunc& f,int n);
vec value(const dualVec& v);
vec derivative(const dualVec& v);

void withRespectTo(dualVec& V,uint i);

}
}

#endif // DUAL_NUMBERS_H
