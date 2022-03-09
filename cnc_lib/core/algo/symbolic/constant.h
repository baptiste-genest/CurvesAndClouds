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
    scalar_property p;
    friend class Expression;
    Constant(scalar_property p);
    static constexpr scalar epsilon = 1e-10;
public:
    cscalar getValue() const;
    Constant(cscalar v);

    bool operator==(const Symbol& other) const override;
    virtual Expression differentiate(const Variable& x) const override;
    virtual cscalar evaluate(const ValuationSystem& V) const override;
    virtual std::string print() const override;
    virtual Expression expand() const override;
    virtual Expression compose(const Variable& x,const Expression& e) const override;

    operator Expression() const;

    static Expression Zero();
    static Expression One();
};

cscalar FixValue(Expression E);

Expression operator-(Expression E);

}

}


#endif // CONSTANT_H
