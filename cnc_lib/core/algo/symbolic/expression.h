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

namespace cnc {

namespace symbolic {

class Expression;

class Symbol;
class Variable;

using SymbolRef = std::shared_ptr<Symbol>;

using VariableId = int;

using varSet = std::set<VariableId>;

using ValuationPair = std::pair<VariableId,scalar>;
struct ValuationSystem {
    ValuationSystem(std::initializer_list<ValuationPair> mv);
    ValuationSystem(const std::vector<ValuationPair>& mv);
    std::map<VariableId,scalar> mapping;
    scalar evaluate(const VariableId& id) const;
};

class Symbol{
    public:
    virtual bool isSum() const {return false;}
    virtual bool isProduct() const {return false;}
    virtual bool isQuotient() const {return false;}

    virtual Expression differentiate(const Variable& x) const = 0;
    virtual cscalar evaluate(const ValuationSystem& V) const = 0;
    virtual std::string print() const = 0;
    virtual Expression expand() const = 0;
    virtual Expression compose(const Variable& x,const Expression& e) const = 0;
};

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
