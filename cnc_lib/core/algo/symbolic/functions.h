#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "variable.h"
#include "constant.h"
#include "term.h"

namespace cnc {
namespace symbolic {

namespace functions{
enum name {
    exp,
    cos,
    sin,
    sigmoid,
    re,
    im,
    log
};
}

class Function : public Term {
private:
    functions::name func_name;
    Function(functions::name f,Expression e);
    Expression arg;

public:
    //bool operator==(const Symbol& other) const override;
    virtual matchResult matchWith(const Expression& other) const override;
    virtual Expression differentiate(const Variable& x) const override;
    virtual cscalar evaluate(const ValuationSystem& V) const override;
    virtual std::string print() const override;
    virtual Expression expand() const override;
    virtual Expression compose(const Variable& x,const Expression& e) const override;
    virtual Expression simplify() const override;
    //virtual void treePrint(uint padding = 0) const override;

    friend Expression exp(Expression e);
    friend Expression cos(Expression e);
    friend Expression sin(Expression e);
    friend Expression sigmoid(Expression e);
    friend Expression Re(Expression e);
    friend Expression Im(Expression e);
    friend Expression log(Expression e);
};

Expression exp(Expression e);
Expression cos(Expression e);
Expression sin(Expression e);
Expression sigmoid(Expression e);
Expression Re(Expression e);
Expression Im(Expression e);
Expression log(Expression e);

}
}

#endif // FUNCTIONS_H
