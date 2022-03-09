#include "constant.h"

cnc::symbolic::Constant::Constant(scalar_property prop) : p(prop)
{
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
    p = zero;
}

cnc::cscalar cnc::symbolic::Constant::getValue() const
{
    return value;
}

cnc::symbolic::Constant::Constant(cnc::cscalar v) : value(v),p(fixed)
{

}

bool cnc::symbolic::Constant::operator==(const cnc::symbolic::Symbol &o) const
{
    const auto& other = static_cast<const Constant&>(o);
    return std::abs(value - other.value) < epsilon;
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
    E.p = p;
    return E;
}

cnc::symbolic::Expression cnc::symbolic::Constant::compose(const cnc::symbolic::Variable &, const cnc::symbolic::Expression &) const
{
    return getExpression(*this,{});
}

cnc::symbolic::Constant::operator Expression() const
{
    auto E = Expression(std::make_shared<Constant>(*this),{});
    E.p = p;
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
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(x,y,sub)),Union(x.getVariables(),y.getVariables()));
}

cnc::symbolic::Expression cnc::symbolic::operator/(cnc::symbolic::Expression x, cnc::symbolic::Expression y)
{
    if (y.getScalarProperty() == one)
        return x;
    if (x.getScalarProperty() == zero)
        return Constant::Zero();
    if (y.getScalarProperty() == zero)
        throw Cnc_error("Division by zero.");
    return Expression(std::make_shared<BinaryOperator>(BinaryOperator(x,y,quotient)),Union(x.getVariables(),y.getVariables()));
}

cnc::symbolic::Expression cnc::symbolic::operator-(cnc::symbolic::Expression E){
    return E*Constant(-1.);
}

cnc::cscalar cnc::symbolic::Expression::fixValue()
{
    if (getScalarProperty() == other)
        throw Cnc_error("Can't fix variable value");
    return std::static_pointer_cast<Constant>(ref)->getValue();
}

cnc::symbolic::Expression::Expression(cnc::cscalar x)
{
    if (std::abs(x-scalar(1.)) < 1e-10){
        ref = std::make_shared<Constant>(Constant(one));
        p = one;
    }
    else {
        ref = std::make_shared<Constant>(Constant(x));
        p = fixed;
    }
}

cnc::symbolic::Expression::Expression(cnc::scalar x)
{
    if (std::abs(x-1.) < 1e-10){
        ref = std::make_shared<Constant>(Constant(one));
        p = one;
    }
    else {
        ref = std::make_shared<Constant>(Constant(x));
        p = fixed;
    }
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
