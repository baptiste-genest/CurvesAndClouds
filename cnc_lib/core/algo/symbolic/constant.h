#ifndef CONSTANT_H
#define CONSTANT_H

#include "expression.h"
#include "variable.h"
#include "operators.h"
#include "term.h"

namespace cnc {

namespace symbolic {

class Constant : public Term
{
private:
    cscalar value;
    friend class Expression;
    Constant(scalar_property p);
    static constexpr scalar epsilon = 1e-10;
public:
    cscalar getValue() const;
    Constant(cscalar v);
    Constant(scalar v);
    Constant(int v);

    virtual matchResult matchWith(const Expression& o) const override;
    virtual Expression differentiate(const Variable& x) const override;
    virtual cscalar evaluate(const ValuationSystem& V) const override;
    virtual Expression simplify() const override;
    virtual std::string print() const override;
    virtual Expression expand() const override;
    virtual Expression compose(const Variable& x,const Expression& e) const override;
    //virtual void treePrint(uint padding = 0) const override;

    operator Expression() const;

    static Expression Zero();
    static Expression One();
};

cscalar FixValue(Expression E);

Expression operator-(Expression E);

}

}


#endif // CONSTANT_H
