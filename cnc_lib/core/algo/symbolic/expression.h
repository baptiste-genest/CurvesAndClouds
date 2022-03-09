#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "cnc_types.h"
#include "cnc_error.h"
#include <memory>
#include <map>
#include <vector>
#include <typeinfo>
#include <set>
#include "core/algo/algo.h"
#include "symbol.h"

namespace cnc {
namespace symbolic {

enum scalar_property{
    zero,
    one,
    fixed,
    other
};

class Expression
{
private:
    SymbolRef ref;
    scalar_property p = other;
    friend class Constant;
    varSet variables_involved;

public:
    cscalar fixValue();
    Expression();
    Expression(SymbolRef r,const varSet& VI) : ref(r),variables_involved(VI){}
    Expression(cscalar x);
    Expression(scalar x);

    bool operator==(const Expression& other) const;

    Expression operator()(VariableId id) const;

    const varSet& getVariables() const;

    cscalar operator()(const ValuationSystem& V) const;

    scalar_property getScalarProperty() const;

    virtual Expression differentiate(const Variable& x) const;
    virtual cscalar evaluate(const ValuationSystem& V) const;
    virtual std::string print() const;
    virtual Expression expand() const;
    virtual Expression compose(const Variable& x,const Expression& e) const;
};

template<typename T>
Expression getExpression(const T& x,const varSet& V){
    return Expression(std::make_shared<T>(x),V);
}
varSet Union(const varSet& A,const varSet& B);

}


}


#endif // EXPRESSION_H
