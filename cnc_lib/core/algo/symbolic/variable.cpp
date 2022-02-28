#include "variable.h"


cnc::symbolic::Expression::Expression()
{
    ref = std::make_shared<Variable>();
}


cnc::symbolic::scalar_property cnc::symbolic::Expression::getScalarProperty() const
{
    return p;
}

int cnc::symbolic::Variable::count = 0;
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
    return getExpression(*this);
}

cnc::symbolic::Expression cnc::symbolic::Variable::compose(const cnc::symbolic::Variable &x, const cnc::symbolic::Expression &e) const
{
    if (x.id == id)
        return e;
    return getExpression(*this);
}

cnc::symbolic::ValuationPair cnc::symbolic::Variable::operator==(cnc::scalar x) const
{
    return {id,x};
}

cnc::symbolic::Variable::operator Expression() const
{
    return Expression(std::make_shared<Variable>(*this));
}

cnc::symbolic::Expression cnc::symbolic::Expression::differentiate(const cnc::symbolic::Variable& x) const
{
    return ref->differentiate(x);
}
