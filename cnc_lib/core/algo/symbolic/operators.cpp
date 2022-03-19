#include "operators.h"

cnc::symbolic::BinaryOperator::BinaryOperator(cnc::symbolic::Expression x, cnc::symbolic::Expression y,bin_operators t) : a(x),b(y),type(t)
{

}

cnc::cscalar cnc::symbolic::BinaryOperator::evaluate(const cnc::symbolic::ValuationSystem &V) const
{
    cscalar va = a.evaluate(V);
    cscalar vb = b.evaluate(V);
    cscalar r;
    switch (type){
    case cnc::symbolic::exponentiation:
        r = std::pow(va,vb);
        break;
    case cnc::symbolic::sum:
        r = va+vb;
        break;
    case cnc::symbolic::product:
        r = va*vb;
        break;
    case cnc::symbolic::sub:
        r = va-vb;
        break;
    case cnc::symbolic::quotient:
    {
        if (std::abs(va) < 1e-10 && std::abs(vb) < 1e-10)
            return 1.;
        r = va/vb;
        break;
    }
    }
    return r;
}

std::string cnc::symbolic::BinaryOperator::print() const
{
    static constexpr char ope[4] = {'+','*','-','/'};
    if (type == exponentiation)
        return "pow(" + a.print() + "," + b.print() +")";
    return "(" +  a.print() + ")"  + ope[int(type)] + "(" + b.print() + ")";
}

cnc::symbolic::Expression cnc::symbolic::BinaryOperator::expand() const
{
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(a.expand(),b.expand(),type)),Union(a.getVariables(),b.getVariables()));
}

cnc::symbolic::Expression cnc::symbolic::BinaryOperator::compose(const cnc::symbolic::Variable &x, const cnc::symbolic::Expression &e) const
{
    auto A = a.compose(x,e);
    auto B = b.compose(x,e);
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(A,B,type)),Union(A.getVariables(),B.getVariables()));
}

cnc::symbolic::matchResult cnc::symbolic::BinaryOperator::matchWith(const Expression& o) const
{
    const auto& other = static_cast<const BinaryOperator&>(*o.getRef());
    if (type != other.type)
        return {false,{}};
    {
        auto M1 = a.matchWith(other.a);
        auto M2 = b.matchWith(other.b);
        auto merge = Expression::mergeMatch(M1,M2);
        if (merge.first)
            return merge;
    }
    if (isCommutative()){
        auto M1 = a.matchWith(other.b);
        auto M2 = b.matchWith(other.a);
        auto merge = Expression::mergeMatch(M1,M2);
        if (merge.first)
            return merge;
    }
    return {false,{}};
}

cnc::symbolic::Expression cnc::symbolic::BinaryOperator::simplify() const
{
    auto as = a.simplify();
    auto bs = b.simplify();
    switch (type) {
    case cnc::symbolic::sum:
        return as + bs;
    case cnc::symbolic::product:
        return as * bs;
    case cnc::symbolic::sub:
        return as - bs;
    case cnc::symbolic::quotient:
        return as / bs;
    case cnc::symbolic::exponentiation:
        return pow(as,bs);
    }
}

void cnc::symbolic::BinaryOperator::treePrint(uint padding) const
{
    static constexpr char ope[4] = {'+','*','-','/'};
    if (type == exponentiation)
        std::cout << print() << std::endl;
    else {
        a.treePrint(padding+1);
        std::cout << algo::repeat("  ",padding) + ope[int(type)] << std::endl;
        b.treePrint(padding+1);
    }
}

bool cnc::symbolic::BinaryOperator::isCommutative() const
{
    return int(type) < 2;
}
