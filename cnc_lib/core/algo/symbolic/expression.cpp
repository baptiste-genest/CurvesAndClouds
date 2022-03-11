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

cnc::symbolic::ValuationSystem::ValuationSystem(const std::vector<ValuationPair> &mv)
{
    for (const auto& vp : mv)
        mapping[vp.first] = vp.second;
}

cnc::scalar cnc::symbolic::ValuationSystem::evaluate(const cnc::symbolic::VariableId &id) const
{
    auto e = mapping.find(id);
    if (e == mapping.end())
        throw Cnc_error("Can't evaluate unset variable");
    return e->second;
}

cnc::symbolic::varSet cnc::symbolic::Union(const cnc::symbolic::varSet &A, const cnc::symbolic::varSet &B)
{
    auto S = A;
    for (const auto& x : B)
        S.insert(x);
    return S;
}

bool cnc::symbolic::Expression::operator==(const cnc::symbolic::Expression & other) const
{
    auto s = other.ref;
    if (typeid(s) != typeid (ref))
        return false;
    filterMap M;
    return ref->matchWith(s,M);
}
