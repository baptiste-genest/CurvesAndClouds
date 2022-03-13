#include "simplify.h"

cnc::symbolic::Expression cnc::symbolic::Expression::simplify() const
{
    using idiom = Expression;
    static Variable a = Variable::getPlaceholder(); static VariableId aid = a.getId();
    static Variable b = Variable::getPlaceholder(); static VariableId bid = b.getId();
    static Variable c = Variable::getPlaceholder(); static VariableId cid = c.getId();
    static Variable d = Variable::getPlaceholder(); static VariableId did = d.getId();

    Expression E = ref->simplify();
    bool changed = false;
    auto V = E.getVariables();

    {
        static idiom factor = a*b;
        auto M1 = factor.matchWith(E);
        if (M1.first){
            if (M1.second[bid].getScalarProperty() != other){
                E = M1.second[bid].evaluate({})*M1.second[aid];
                changed = true;
            }
        }
    }
    {
        static idiom factor = a*(b*c);
        auto M1 = factor.matchWith(E);
        if (M1.first){
            if (M1.second[bid].getScalarProperty() != other && M1.second[aid].getScalarProperty() != other){
                E = (M1.second[bid].evaluate({})*M1.second[aid].evaluate({}))*M1.second[cid];
                changed = true;
            }
            else if (M1.second[bid].getScalarProperty() != other && M1.second[aid].getScalarProperty() == other){
                E = M1.second[bid].evaluate({})*(M1.second[aid]*M1.second[cid]);
                changed = true;
            }
            else if (M1.second[cid].getScalarProperty() != other && M1.second[aid].getScalarProperty() == other){
                E = M1.second[cid].evaluate({})*(M1.second[aid]*M1.second[bid]);
                changed = true;
            }
        }
    }
    {
        static idiom square = a*a;
        auto M1 = square.matchWith(E);
        if (M1.first){
            E = pow(M1.second[aid],2);
            changed = true;
        }
    }
    {
        static idiom square = a*(a*b);
        auto M1 = square.matchWith(E);
        if (M1.first){
            E = pow(M1.second[aid],2)*M1.second[bid];
            changed = true;
        }
    }
    {
        static idiom inverse = pow(a/b,-1);
        auto M1 = inverse.matchWith(E);
        if (M1.first){
            E = M1.second[bid]/M1.second[aid];
            changed = true;
        }
    }
    {
        static idiom inverse = a/b + c/b;
        auto M1 = inverse.matchWith(E);
        if (M1.first){
            E = (M1.second[aid]+M1.second[cid])/M1.second[bid];
            changed = true;
        }
    }
    {
        static idiom inverse = pow(a/b,c);
        auto M1 = inverse.matchWith(E);
        if (M1.first){
            if (M1.second[cid].getScalarProperty() != other){
                auto n = M1.second[cid].evaluate({}).real();
                if (n < 0){
                    E = pow(M1.second[bid]/M1.second[aid],-n);
                    changed = true;
                }
            }
        }
    }
    {
        static idiom cos_sin = pow(cos(a),2) + pow(sin(a),2);
        auto M1 = cos_sin.matchWith(E);
        if (M1.first){
            E = Constant::One();
            changed = true;
        }
    }
    {
        static idiom cancel = a - a;
        auto M1 = cancel.matchWith(E);
        if (M1.first){
            E = Constant::Zero();
            changed = true;
        }
    }
    {
        static idiom cancel = a / a;
        auto M1 = cancel.matchWith(E);
        if (M1.first){
            E = Constant::One();
            changed = true;
        }
    }
    {
        static idiom cancel = (a/b) * b;
        auto M1 = cancel.matchWith(E);
        if (M1.first){
            E = M1.second[aid];
            changed = true;
        }
    }
    {
        static idiom powproduct = pow(a,b)*a;
        auto M1 = powproduct.matchWith(E);
        if (M1.first){
            E = pow(M1.second[aid],M1.second[bid] + 1);
            changed = true;
        }
    }
    {
        static idiom powsplit = pow(a*b,c);
        auto M1 = powsplit.matchWith(E);
        if (M1.first){
            E = pow(M1.second[aid],M1.second[cid])*pow(M1.second[bid],M1.second[cid]);
            changed = true;
        }
    }
    {
        static idiom powproduct = pow(a,b)*pow(a,c);
        auto M1 = powproduct.matchWith(E);
        if (M1.first){
            E = pow(M1.second[aid],M1.second[bid] + M1.second[cid]);
            changed = true;
        }
    }
    {
        static idiom powproduct = (pow(a,b)*d)*pow(a,c);
        auto M1 = powproduct.matchWith(E);
        if (M1.first){
            E = pow(M1.second[aid],M1.second[bid] + M1.second[cid])*M1.second[did];
            changed = true;
        }
    }
    {
        static idiom powproduct = pow(a,b)/pow(a,c);
        auto M1 = powproduct.matchWith(E);
        if (M1.first){
            E = pow(M1.second[aid],M1.second[bid] - M1.second[cid]);
            changed = true;
        }
    }
    {
        static idiom powpow = pow(pow(a,b),c);
        auto M1 = powpow.matchWith(E);
        if (M1.first){
            E = pow(M1.second[aid],M1.second[bid] * M1.second[cid]);
            changed = true;
        }
    }
    {
        static idiom timestwo = a+a;
        auto M1 = timestwo.matchWith(E);
        if (M1.first){
            E = 2*M1.second[aid];
            changed = true;
        }
    }
    {
        static idiom factor = a+(b*a);
        auto M1 = factor.matchWith(E);
        if (M1.first){
            E = (M1.second[bid]+1)*M1.second[aid];
            changed = true;
        }
    }

    if (changed)
        return E.simplify();
    return E;
}
