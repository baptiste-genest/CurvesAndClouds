#include "variable.h"



cnc::symbolic::scalar_property cnc::symbolic::Expression::getScalarProperty() const
{
    return p;
}

int cnc::symbolic::Variable::count = 0;
int cnc::symbolic::Variable::placeholder_count = 0;
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

bool cnc::symbolic::Variable::matchWith(SymbolRef o, filterMap &M) const
{
    const auto& other = static_cast<const Variable&>(*o);
    if (id < 0){
        auto e = M.find(id);
        if (e == M.end()){
            M[id] = o;
        }
    }
    return (*this) == other;
}

