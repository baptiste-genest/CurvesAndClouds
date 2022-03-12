#ifndef SYMBOL_H
#define SYMBOL_H

#include <memory>
#include <map>
#include <vector>
#include <set>
#include "cnc_types.h"

namespace cnc {

namespace symbolic {

class Expression;

class Symbol;
class Variable;

using SymbolRef = std::shared_ptr<Symbol>;
using VariableId = int;
using varSet = std::set<VariableId>;
using ValuationPair = std::pair<VariableId,scalar>;
using filterMap = std::map<int,Expression>;
using matchResult = std::pair<bool,filterMap>;

enum scalar_property{
    zero,
    one,
    fixed,
    other
};


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

    virtual scalar_property getScalarProperty() const {
        return p;
    }

    virtual matchResult matchWith(const Expression& other) const = 0;
    virtual Expression simplify() const = 0;
    virtual Expression differentiate(const Variable& x) const = 0;
    virtual cscalar evaluate(const ValuationSystem& V) const = 0;
    virtual std::string print() const = 0;
    virtual Expression expand() const = 0;
    virtual Expression compose(const Variable& x,const Expression& e) const = 0;
protected:
    inline void setScalarProperty(scalar_property prop) {
        p = prop;
    }
    scalar_property p = other;
    //static matchResult mergeMatch(const matchResult& A,const matchResult& B);
};

}
}


#endif // SYMBOL_H
