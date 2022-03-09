#ifndef OPERATORS_H
#define OPERATORS_H

#include "expression.h"
#include "variable.h"

namespace cnc {

namespace symbolic {

enum bin_operators {
    sum,
    product,
    sub,
    quotient
};

class BinaryOperator : public Symbol {
public:
    virtual Expression differentiate(const Variable& x) const override ;
    virtual cscalar evaluate(const ValuationSystem& V) const override;
    virtual std::string print() const override;
    virtual Expression expand() const override;
    virtual Expression compose(const Variable& x,const Expression& e) const override;
    bool operator==(const Symbol& other) const override;
    bool isCommutative() const;

private:
    BinaryOperator(Expression x,Expression y,bin_operators t);
    Expression a,b;
    bin_operators type;

    friend Expression operator+(Expression x,Expression y);
    friend Expression operator-(Expression x,Expression y);
    friend Expression operator*(Expression x,Expression y);
    friend Expression operator/(Expression x,Expression y);
};

Expression operator+(Expression x,Expression y);
Expression operator*(Expression x,Expression y);
Expression operator-(Expression x,Expression y);
Expression operator/(Expression x,Expression y);



}

}

#endif // OPERATORS_H
