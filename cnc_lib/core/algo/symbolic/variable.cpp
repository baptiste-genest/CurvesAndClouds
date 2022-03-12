#include "variable.h"

int cnc::symbolic::Variable::count = 0;
int cnc::symbolic::Variable::placeholder_count = 1;
cnc::symbolic::Variable::Variable(const cnc::symbolic::Variable &x)
{
    id = x.id;
}

cnc::symbolic::Variable::Variable(){
    id = count;
    count++;
}

cnc::cscalar cnc::symbolic::Variable::evaluate(const cnc::symbolic::ValuationSystem &V) const
{
    return V.evaluate(id);
}

std::string cnc::symbolic::Variable::print() const
{
    return " [[ "  +  std::to_string(id) + " ]] ";
}

cnc::symbolic::Expression cnc::symbolic::Variable::expand() const
{
    return getExpression(*this,{id});
}

cnc::symbolic::Expression cnc::symbolic::Variable::compose(const cnc::symbolic::Variable &x, const cnc::symbolic::Expression &e) const
{
    if (x.id == id)
        return e;
    return getExpression(*this,{id});
}

cnc::symbolic::VariableId cnc::symbolic::Variable::getId() const
{
    return id;
}

cnc::symbolic::ValuationPair cnc::symbolic::Variable::operator==(cnc::scalar x) const
{
    return {id,x};
}

cnc::symbolic::Variable cnc::symbolic::Variable::getVariableFromId(cnc::symbolic::VariableId id)
{
    if (id >= count)
        throw Cnc_error("Variable doesn't exists");
    Variable V;
    V.id = id;
    return V;
}

cnc::symbolic::Variable cnc::symbolic::Variable::getPlaceholder()
{
    Variable V;
    V.id = -placeholder_count;
    placeholder_count++;
    return V;
}

cnc::symbolic::Variable::operator Expression() const
{
    return Expression(std::make_shared<Variable>(*this),{id});
}

/*
cnc::symbolic::matchResult cnc::symbolic::Symbol::matchWith(SymbolRef a, SymbolRef b)
{
    if (auto v = dynamic_cast<Variable*>(a.get()))
    {
        if (v->getId() < 0) // I'm a placeholder variable
            return a->matchWith(b);
    }
    if (typeid(*a) != typeid (*b))
        return {false,{}};
    return a->matchWith(b);

}
*/
cnc::symbolic::matchResult cnc::symbolic::Expression::matchWith(const Expression &a, const Expression &b)
{
    if (auto v = dynamic_cast<Variable*>(a.ref.get()))
    {
        if (v->getId() < 0) // I'm a placeholder variable
            return a.ref->matchWith(b);
    }
    if (typeid(*a.ref) != typeid (*b.ref))
        return {false,{}};
    return a.ref->matchWith(b);
}


cnc::symbolic::matchResult cnc::symbolic::Expression::matchWith(const Expression &o) const {
    return matchWith(*this,o);
}

cnc::symbolic::matchResult cnc::symbolic::Variable::matchWith(const Expression& o) const
{
    if (id < 0){
        filterMap M;
        M[id] = o;
        return {true,M};
    }
    auto other = static_cast<Variable*>(o.getRef().get());
    return {other->id == id,{}};
}

cnc::symbolic::Expression cnc::symbolic::Variable::simplify() const
{
    return *this;
}

