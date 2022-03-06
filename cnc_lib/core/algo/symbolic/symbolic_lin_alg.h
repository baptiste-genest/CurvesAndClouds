#ifndef SYMBOLIC_LIN_ALG_H
#define SYMBOLIC_LIN_ALG_H

#include "cnc_types.h"
#include "functions.h"
#include "../linear_algebra/lin_alg.h"

namespace cnc {

namespace symbolic {

class smat{
public:
    smat(uint w,uint h);
    Expression& operator()(uint i,uint j);
    cmat evaluate(const ValuationSystem& V) const;
private:
    std::vector<Expression> v;
    uint width,height;
};

class svec{
public:
    svec(){}
    svec(uint n);
    smat jacobian(const std::vector<Variable>& X) const;
    Expression& operator()(uint j);
    cvec evaluate(const ValuationSystem& V) const;
    vec evaluate_real(const ValuationSystem& V) const;
    vec evaluate(const vec& x) const;
    cvec evaluate(const cvec& x) const;
    varSet getVariablesInvolved() const;
    std::string print() const;

private:
    uint h;
    std::vector<Expression> v;
};


}

}
#endif // SYMBOLIC_LIN_ALG_H
