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

cnc::symbolic::matchResult cnc::symbolic::Expression::mergeMatch(const matchResult &A, const matchResult &B)
{
    if (!A.first || !B.first)
        return {false,{}};
    if (A.second.empty())
        return {true,B.second};
    if (B.second.empty())
        return {true,A.second};
    auto M = B.second;
    for (const auto& m : A.second){
        auto bm = B.second.find(m.first);
        if (bm != B.second.end()){
            if (!matchWith(m.second,bm->second).first)
                return {false,{}};
        }
        M.insert(m);
    }
    return {true,M};
}

bool cnc::symbolic::Expression::operator==(const cnc::symbolic::Expression & other) const
{
    if (typeid(*other.ref) != typeid (*ref))
        return false;
    return ref->matchWith(other).first;
}

std::ostream &cnc::symbolic::operator<<(std::ostream &os, const Expression &E){
    os << E.print();
    return os;
}
