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
    virtual bool matchWith(SymbolRef other,filterMap& M) const override;
    //bool operator==(const Symbol& other) const override;
    VariableId getId() const;

    ValuationPair operator==(scalar x) const;
    inline bool operator==(const Variable& other) const{
        return id == other.id;
    }

    operator Expression() const;
    static Variable getVariableFromId(VariableId id);

private:
    static Variable getPlaceholder();
    static int count;
    static int placeholder_count;
    VariableId id;
};

}
}

#endif // VARIABLE_H
