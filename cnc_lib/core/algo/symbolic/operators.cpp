#include "operators.h"

cnc::symbolic::BinaryOperator::BinaryOperator(cnc::symbolic::Expression x, cnc::symbolic::Expression y,bin_operators t) : a(x),b(y),type(t)
{

}

cnc::symbolic::Expression cnc::symbolic::BinaryOperator::differentiate(const cnc::symbolic::Variable &x) const
{
    auto da = a.differentiate(x);
    auto db = b.differentiate(x);
    switch (type) {
    case cnc::symbolic::sum:
        return da + db;
    case cnc::symbolic::product:
        return da*b + a*db;
    case cnc::symbolic::sub:
        return da - db;
    case cnc::symbolic::quotient:
        return (da*b - a*db)/(b*b);
    }
}

cnc::cscalar cnc::symbolic::BinaryOperator::evaluate(const cnc::symbolic::ValuationSystem &V) const
{
    cscalar va = a.evaluate(V);
    cscalar vb = b.evaluate(V);
    cscalar r;
    switch (type){
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
        r = va/vb;
        break;
    }
    return r;
}

std::string cnc::symbolic::BinaryOperator::print() const
{
    static const char ope[4] = {'+','*','-','/'};
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
