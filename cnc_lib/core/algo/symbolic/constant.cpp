#include "constant.h"

cnc::symbolic::Constant::Constant(scalar_property prop)
{
    p = prop;
    switch (prop){
    case cnc::symbolic::zero:
        value = 0.;
        break;
    default:
        value = 1.;
    }
}

cnc::symbolic::Expression::Expression()
{
    ref = Constant::Zero().ref;
}

cnc::cscalar cnc::symbolic::Constant::getValue() const
{
    return value;
}

cnc::symbolic::Constant::Constant(cnc::cscalar v) : value(v)
{
    p = fixed;
    if (std::abs(v-scalar(1.)) < 1e-10){
        p = one;
    }
    else if (std::abs(v) < 1e-20){
        p = zero;
    }
}

cnc::symbolic::Constant::Constant(scalar v) : Constant(cscalar(v))
{
}

cnc::symbolic::Constant::Constant(int v)
{
    value = v;
    p = fixed;
    if (v == 1)
        p = one;
    else if (v == 0)
        p = zero;
}

cnc::symbolic::matchResult cnc::symbolic::Constant::matchWith(const Expression& o) const
{
    const auto& other = static_cast<const Constant&>(*o.getRef());
    return {std::abs(value - other.value) < epsilon,{}};
}

cnc::symbolic::Expression cnc::symbolic::Constant::differentiate(const cnc::symbolic::Variable &) const
{
    return Constant::Zero();
}

cnc::cscalar cnc::symbolic::Constant::evaluate(const cnc::symbolic::ValuationSystem &) const
{
    switch (p){
        case zero:
            return 0.;
        case one:
            return 1.;
        default:
            return value;
    }
}

cnc::symbolic::Expression cnc::symbolic::Constant::simplify() const
{
    return getExpression(*this,{});
}

std::string cnc::symbolic::Constant::print() const
{
    switch (p){
        case zero:
            return " 0 ";
        break;
        case one:
            return " 1 ";
        break;
        default:
            return ' ' + std::to_string(value.real()) + (std::abs(value.imag()) > 1e-10 ? (" + i" + std::to_string(value.imag())): " ") ;
    }
}

cnc::symbolic::Expression cnc::symbolic::Constant::expand() const
{
    auto E = Expression(std::make_shared<Constant>(*this),{});
    return E;
}

cnc::symbolic::Expression cnc::symbolic::Constant::compose(const cnc::symbolic::Variable &, const cnc::symbolic::Expression &) const
{
    return getExpression(*this,{});
}

cnc::symbolic::Constant::operator Expression() const
{
    auto E = Expression(std::make_shared<Constant>(*this),{});
    return E;
}

cnc::symbolic::Expression cnc::symbolic::Constant::Zero()
{
    return Constant(zero);
}

cnc::symbolic::Expression cnc::symbolic::Constant::One()
{
    return Constant(one);
}

cnc::symbolic::Expression cnc::symbolic::Variable::differentiate(const cnc::symbolic::Variable &x) const
{
    if (x.id == id)
        return Constant::One();
    else
        return Constant::Zero();
}

cnc::symbolic::Expression cnc::symbolic::operator+(cnc::symbolic::Expression x, cnc::symbolic::Expression y){
    if (x.getScalarProperty() == zero)
        return y;
    if (y.getScalarProperty() == zero)
        return x;
    if (x.getScalarProperty() != other && y.getScalarProperty() != other)
        return Constant(x.fixValue() + y.fixValue());
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(x,y,sum)),Union(x.getVariables(),y.getVariables()));
}

cnc::symbolic::Expression cnc::symbolic::operator*(cnc::symbolic::Expression x, cnc::symbolic::Expression y)
{
    if (x.getScalarProperty() == one)
        return y;
    if (y.getScalarProperty() == one)
        return x;
    if (x.getScalarProperty() == zero || y.getScalarProperty() == zero)
        return Constant::Zero();
    if (x.getScalarProperty() != other && y.getScalarProperty() != other)
        return Constant(x.fixValue()*y.fixValue());
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(x,y,product)),Union(x.getVariables(),y.getVariables()));
}

cnc::symbolic::Expression cnc::symbolic::operator-(cnc::symbolic::Expression x, cnc::symbolic::Expression y)
{
    if (x.getScalarProperty() == zero)
        return y*Constant(-1.);
    if (y.getScalarProperty() == zero)
        return x;
    if (x.getScalarProperty() == one && y.getScalarProperty() == one)
        return Constant::Zero();
    if (x.getScalarProperty() != other && y.getScalarProperty() != other)
        return Constant(x.fixValue()-y.fixValue());
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(x,y,sub)),Union(x.getVariables(),y.getVariables()));
}

cnc::symbolic::Expression cnc::symbolic::operator/(cnc::symbolic::Expression x, cnc::symbolic::Expression y)
{
    if (y.getScalarProperty() == one)
        return x;
    if (x.getScalarProperty() == one)
        return pow(y,-1);
    if (x.getScalarProperty() == zero)
        return Constant::Zero();
    if (y.getScalarProperty() == zero)
        throw Cnc_error("Division by zero.");
    if (x.getScalarProperty() != other && y.getScalarProperty() != other)
        return Constant(x.fixValue()/y.fixValue());
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(x,y,quotient)),Union(x.getVariables(),y.getVariables()));
}

cnc::symbolic::Expression cnc::symbolic::operator-(cnc::symbolic::Expression E){
    return E*Constant(-1.);
}

cnc::symbolic::Expression cnc::symbolic::pow(Expression e, Expression n)
{
    if (n.getScalarProperty() == one)
        return e;
    if (n.getScalarProperty() == zero || e.getScalarProperty() == one)
        return Constant::One();
    if (n.getScalarProperty() != other && e.getScalarProperty() != other)
        return std::pow(e.fixValue(),n.fixValue());
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(e,n,exponentiation)),Union(e.getVariables(),n.getVariables()));
}

cnc::cscalar cnc::symbolic::Expression::fixValue()
{
    if (getScalarProperty() == other)
        throw Cnc_error("Can't fix variable value");
    return ref->evaluate({});
}

cnc::symbolic::Expression::Expression(cnc::cscalar x)
{
    if (std::abs(x-scalar(1.)) < 1e-10){
        ref = std::make_shared<Constant>(Constant(one));
    }
    else if (std::abs(x) < 1e-10){
        ref = std::make_shared<Constant>(Constant(zero));
    }
    else {
        ref = std::make_shared<Constant>(Constant(x));
    }
}

cnc::symbolic::Expression::Expression(scalar x) : Expression(cscalar(x))
{
}

cnc::symbolic::Expression::Expression(int n)
{
    ref = std::make_shared<Constant>(n);
}

const cnc::symbolic::varSet &cnc::symbolic::Expression::getVariables() const
{
    return variables_involved;
}

cnc::cscalar cnc::symbolic::Expression::operator()(const cnc::symbolic::ValuationSystem &V) const
{
    return evaluate(V);
}

cnc::symbolic::Expression cnc::symbolic::Expression::operator()(cnc::symbolic::VariableId id) const
{
    return differentiate(Variable::getVariableFromId(id));
}

cnc::symbolic::Expression cnc::symbolic::Expression::differentiate(const cnc::symbolic::Variable& x) const
{
    if (variables_involved.find(x.getId()) == variables_involved.end())
        return Constant::Zero();
    return ref->differentiate(x);
}
