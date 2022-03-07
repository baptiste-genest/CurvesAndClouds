#ifndef SYMBOLIC_LIN_ALG_H
#define SYMBOLIC_LIN_ALG_H

#include "cnc_types.h"
#include "functions.h"
#include "../linear_algebra/lin_alg.h"

namespace cnc {

namespace symbolic {

class SymbolicArray {
public:
    SymbolicArray(uint n) : size(n){
        v.resize(n);
    }
    inline Expression& operator()(uint i){
        return v[i];
    }
    inline const Expression& operator()(uint i) const{
        return v[i];
    }
    varSet getVariablesInvolved() const;
protected:
    std::vector<Expression> v;
    uint size;
};

class smat : public SymbolicArray{
public:
    smat(uint w,uint h);
    Expression& operator()(uint i,uint j){
        return v[j*width+i];
    }
    const Expression& operator()(uint i,uint j) const{
        return v[j*width+i];
    }
    mat evaluate(const ValuationSystem& V) const;
    mat operator()(const vec& x) const;
    std::string print() const;
    smat transpose() const;
    smat operator*(const smat& other);
    uint getWidth() const {
        return width;
    }
    uint getHeight() const {
        return height;
    }
protected:
    uint width,height;
};

class svec : public SymbolicArray{
public:
    svec(uint n);
    inline Expression& operator()(uint i){
        return v[i];
    }
    smat jacobian(const std::vector<Variable>& X) const;
    smat jacobian() const;
    vec operator()(const vec& x) const;
    vec evaluate(const ValuationSystem& V) const;
    std::string print() const;
};


}

}
#endif // SYMBOLIC_LIN_ALG_H
