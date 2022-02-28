#ifndef VARIABLE_H
#define VARIABLE_H

#include "expression.h"
#include "cnc_error.h"
#include "term.h"

namespace cnc{
namespace symbolic {

class Variable : public Term {
public:
    Variable(const Variable& x);
    Variable();
    virtual Expression differentiate(const Variable& x) const override;
    virtual cscalar evaluate(const ValuationSystem& V) const override;
    virtual std::string print() const override;
    virtual Expression expand() const override;
    virtual Expression compose(const Variable& x,const Expression& e) const override;

    ValuationPair operator==(scalar x) const;

    operator Expression() const;
private:
    static int count;
    VariableId id;
};

}
}

#endif // VARIABLE_H
