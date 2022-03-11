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
using filterMap = std::map<int,SymbolRef>;
using matchResult = std::pair<bool,filterMap>;

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

    virtual matchResult matchWith(SymbolRef other) const = 0;

    virtual Expression differentiate(const Variable& x) const = 0;
    virtual cscalar evaluate(const ValuationSystem& V) const = 0;
    virtual std::string print() const = 0;
    virtual Expression expand() const = 0;
    virtual Expression compose(const Variable& x,const Expression& e) const = 0;
protected:
    static bool matchCompatibily(const matchResult& A,const matchResult& B);
};

inline bool Symbol::matchCompatibily(const matchResult &A, const matchResult &B)
{
    if (!A.first || !B.first)
        return false;
    if (A.second.empty() || B.second.empty())
        return true;
    for (const auto& m : A.second){
        auto bm = B.second.find(m.first);
        if (bm != B.second.end())
            if (!(*m.second).matchWith((bm)->second).first)
                return false;
    }
    for (const auto& m : B.second){
        auto am = A.second.find(m.first);
        if (am != A.second.end())
            if (!(*m.second).matchWith(am->second).first)
                return false;
    }
    return true;
}

}
}


#endif // SYMBOL_H
