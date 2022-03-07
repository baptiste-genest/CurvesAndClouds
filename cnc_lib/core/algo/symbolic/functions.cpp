#include "functions.h"

cnc::symbolic::Function::Function(cnc::symbolic::functions::name f, cnc::symbolic::Expression e) : func_name(f),arg(e)
{

}

cnc::symbolic::Expression cnc::symbolic::Function::differentiate(const cnc::symbolic::Variable &x) const
{
    switch (func_name) {
    case cnc::symbolic::functions::re:
        return arg.differentiate(x);
    case cnc::symbolic::functions::im:
        return arg.differentiate(x)*Constant(cscalar(0.,1.));
    case cnc::symbolic::functions::cos:
        return sin(arg)*arg.differentiate(x)*Constant(-1.);
    case cnc::symbolic::functions::sin:
        return cos(arg)*arg.differentiate(x);
    case cnc::symbolic::functions::exp:
        return exp(arg)*arg.differentiate(x);
    case cnc::symbolic::functions::sigmoid:
        return (sigmoid(arg)*(Constant(1.)-sigmoid(arg)))*arg.differentiate(x);
    }
}

cnc::cscalar cnc::symbolic::Function::evaluate(const cnc::symbolic::ValuationSystem &V) const
{
    cscalar x = arg.evaluate(V);
    switch (func_name) {
    case cnc::symbolic::functions::re:
        return x.real();
    case cnc::symbolic::functions::im:
        return x.imag();
    case cnc::symbolic::functions::cos:
        return std::cos(x);
    case cnc::symbolic::functions::sin:
        return std::sin(x);
    case cnc::symbolic::functions::exp:
        return std::exp(x);
    case cnc::symbolic::functions::sigmoid:
        return scalar(1)/(scalar(1)+std::exp(-x));
    }
}

std::string cnc::symbolic::Function::print() const
{
    std::string argstr = arg.print();
    static const std::string names[6] = {
        "exp",
        "cos",
        "sin",
        "sigmoid",
        "Re",
        "Im"
    };
    return names[int(func_name)] + "(" + argstr + ")";
}

cnc::symbolic::Expression cnc::symbolic::Function::expand() const
{
    return Expression(std::make_shared<Function>(Function(func_name,arg.expand())),arg.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::Function::compose(const cnc::symbolic::Variable &x, const cnc::symbolic::Expression &e) const
{
    return Expression(std::make_shared<Function>(Function(func_name,arg.compose(x,e))),Union(e.getVariables(),arg.getVariables()));
}

cnc::symbolic::Expression cnc::symbolic::exp(cnc::symbolic::Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::exp,e)),e.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::cos(cnc::symbolic::Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::cos,e)),e.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::sin(cnc::symbolic::Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::sin,e)),e.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::sigmoid(cnc::symbolic::Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::sigmoid,e)),e.getVariables());
}

cnc::symbolic::Power::Power(cnc::symbolic::Expression e, int p) : arg(e),n(p)
{
}

cnc::symbolic::Expression cnc::symbolic::Power::differentiate(const cnc::symbolic::Variable &x) const
{
    return Constant(n)*arg.differentiate(x)*pow(arg,n-1);
}

cnc::cscalar cnc::symbolic::Power::evaluate(const cnc::symbolic::ValuationSystem &V) const
{
    return std::pow(arg.evaluate(V),n);
}

std::string cnc::symbolic::Power::print() const
{
    return "pow(" + arg.print() + "," + std::to_string(n)+")";
}

cnc::symbolic::Expression cnc::symbolic::Power::expand() const
{
    return getExpression(*this,arg.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::Power::compose(const cnc::symbolic::Variable &x, const cnc::symbolic::Expression &e) const
{
    return pow(arg.compose(x,e),n);
}

cnc::symbolic::Expression cnc::symbolic::pow(cnc::symbolic::Expression e, int n)
{
    if (n == 1)
        return e;
    if (n == 0)
        return Constant(1.);
    return Expression(std::make_shared<Power>(Power(e,n)),e.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::Re(cnc::symbolic::Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::re,e)),e.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::Im(cnc::symbolic::Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::im,e)),e.getVariables());
}
