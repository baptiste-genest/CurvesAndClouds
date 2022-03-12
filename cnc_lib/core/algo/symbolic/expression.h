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

class Expression
{
private:
    SymbolRef ref;
    friend class Constant;
    varSet variables_involved;

public:
    cscalar fixValue();
    Expression();
    Expression(SymbolRef r,const varSet& VI) : ref(r),variables_involved(VI){}
    Expression(cscalar x);
    Expression(scalar x);
    Expression(int n);

    bool operator==(const Expression& other) const;

    matchResult matchWith(const Expression& o) const;

    Expression operator()(VariableId id) const;

    const varSet& getVariables() const;

    cscalar operator()(const ValuationSystem& V) const;


    Expression simplify() const;

    inline scalar_property getScalarProperty() const {
        return ref->getScalarProperty();
    }

    virtual Expression differentiate(const Variable& x) const;
    virtual cscalar evaluate(const ValuationSystem& V) const;
    virtual std::string print() const;
    virtual Expression expand() const;
    virtual Expression compose(const Variable& x,const Expression& e) const;
    static matchResult matchWith(const Expression& a,const Expression& b);

    static matchResult mergeMatch(const matchResult& a,const matchResult& b);

    inline SymbolRef getRef() const {
        return ref;
    }

    friend std::ostream& operator<<(std::ostream& os,const Expression& E);
};

std::ostream& operator<<(std::ostream& os,const Expression& E);

template<typename T>
Expression getExpression(const T& x,const varSet& V){
    return Expression(std::make_shared<T>(x),V);
}
varSet Union(const varSet& A,const varSet& B);

}


}


#endif // EXPRESSION_H
