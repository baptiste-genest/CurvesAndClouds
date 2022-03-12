#include "functions.h"

cnc::symbolic::Function::Function(cnc::symbolic::functions::name f, cnc::symbolic::Expression e) : func_name(f),arg(e)
{

}

cnc::symbolic::Expression cnc::symbolic::Function::differentiate(const cnc::symbolic::Variable &x) const
{
    switch (func_name) {
    case cnc::symbolic::functions::log:
        return arg.differentiate(x)/arg;
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
    case cnc::symbolic::functions::log:
        return std::log(x);
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
    static const std::string names[7] = {
        "exp",
        "cos",
        "sin",
        "sigmoid",
        "Re",
        "Im",
        "log"
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

cnc::symbolic::Expression cnc::symbolic::Function::simplify() const
{
    auto args = arg.simplify();
    return getExpression(Function(func_name,args),args.getVariables());
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

cnc::symbolic::Expression cnc::symbolic::Re(cnc::symbolic::Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::re,e)),e.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::Im(cnc::symbolic::Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::im,e)),e.getVariables());
}

cnc::symbolic::matchResult cnc::symbolic::Function::matchWith(const Expression &o) const
{
    const auto& other = static_cast<const Function&>(*o.getRef());
    if (func_name != other.func_name)
        return {false,{}};
    return arg.matchWith(other.arg);
}


cnc::symbolic::Expression cnc::symbolic::log(Expression e)
{
    return Expression(std::make_shared<Function>(Function(functions::log,e)),e.getVariables());
}

cnc::symbolic::Expression cnc::symbolic::BinaryOperator::differentiate(const cnc::symbolic::Variable &x) const
{
    auto da = a.differentiate(x);
    auto db = b.differentiate(x);
    switch (type) {
    case cnc::symbolic::exponentiation:
        if (b.getScalarProperty() == fixed){
            auto n = b.evaluate({});
            return n*pow(a,n-1)*da;
        }
        else {
            return pow(a,b-1)*(b*da + a*log(a)*db);
        }
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
