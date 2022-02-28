#include "expression.h"

cnc::cscalar cnc::symbolic::Expression::evaluate(const ValuationSystem& V) const
{
    return ref->evaluate(V);
}

std::string cnc::symbolic::Expression::print() const
{
    return ref->print();
}

cnc::symbolic::Expression cnc::symbolic::Expression::expand() const
{
    return ref->expand();
}

cnc::symbolic::Expression cnc::symbolic::Expression::compose(const cnc::symbolic::Variable &x, const cnc::symbolic::Expression &e) const
{
    return ref->compose(x,e);
}

cnc::symbolic::ValuationSystem::ValuationSystem(std::initializer_list<ValuationPair> mv)
{
    for (const auto& vp : mv)
        mapping[vp.first] = vp.second;
}

cnc::cscalar cnc::symbolic::ValuationSystem::evaluate(const cnc::symbolic::VariableId &id) const
{
    auto e = mapping.find(id);
    if (e == mapping.end())
        throw Cnc_error("Can't evaluate unset variable");
    return e->second;

}
