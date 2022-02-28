#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::calculus;
using namespace cnc::symbolic;


using matfunction = std::function<mat(const vec&)>;

Variable X[3];
const Variable& x = X[0];
const Variable& y = X[1];
const Variable& z = X[2];

Expression f = 2.*y*(y*y - 3.*x*x)*(1.-z*z) + pow(x*x + y*y,2) - (9.*z*z - 1.)*(1.-z*z);

Expression Grad[3];
Expression Hessian[3][3];

ValuationSystem toValuation(const vec& x){
    ValuationSystem V = {
        {X[0]==x(0)},
        {X[1]==x(1)},
        {X[2]==x(2)}
    };
    return V;
}

scalar func(const vec& x){
    return f.evaluate(toValuation(x));
}

vec grad(const vec& x){
    vec g(3);
    auto V = toValuation(x);

    for (uint i = 0;i<3;i++)
        g(i) = Grad[i].evaluate(V);
    return g;
}

mat hessian(const vec& x){
    mat H(3);
    auto V = toValuation(x);

    for (uint j = 0;j<3;j++)
        for (uint i = 0;i<3;i++)
            H(i,j) = Hessian[j][i].evaluate(V);
    return H;
}


//std::vector<vec> levelset(const scalar_function& f,const matfunction& H,const vector_function& grad,const vec& x0,scalar dx,uint n){
std::vector<vec> levelset(const vec& x0,scalar dx,uint n){
    std::vector<vec> S(n);
    auto x = x0;
    for (uint i = 0;i<n;i++){
        S[i] = x;
        std::cout << func(x) << std::endl;
        auto Ha = hessian(x);
        auto ga = grad(x);
        auto h = algo::IterativeSolvers::ConjugateGradient(Ha,ga*(-2.),vec(3),1e-10);
        x += h;
        //std::cout << x << std::endl;
    }
    return S;
}

/*
vec getRoot(vec x){
    while (std::abs(func(x)) > 1e-6){
        std::cout << hessian(x) << std::endl;
        std::cout << "field value: " << std::abs(func(x)) << std::endl;
        x = x + algo::IterativeSolvers::ConjugateGradient(hessian(x),-grad(x),vec(3),1e-10,true);
    }
    return x;
}
*/

int main(int argc, char *argv[])
{
    auto f2 = f*f;
    for (uint i = 0;i<3;i++)
        Grad[i] = f.differentiate(X[i]);

    for (uint j = 0;j<3;j++)
        for (uint i = 0;i<3;i++)
            Hessian[i][j] = Grad[i].differentiate(X[j]);

    //std::cout << f.print() << std::endl;
    //auto x0 = getRoot();
    auto x0 = linear_utils::vec3(0,0,1);
    auto g = grad(x0).normalize();
    auto P = (chaskal::Id<scalar>(3)-mat(g)*g.transpose());
    auto perp = P*(algo::stat::random_var::random_vec_in_sphere(1.,3));
    x0 += perp*0.001;
    std::cout << P*hessian(x0) << std::endl;

    /*
    std::cout << func(x0) << std::endl;
    std::cout << grad(x0) << std::endl;
    std::cout << hessian(x0) << std::endl;
    */

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);


    PlotTab* T =w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 5;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);

    auto C = levelset(x0,0.1,500);
    for (auto& x : C)
        x = linear_utils::vec2(x(0),x(1));
    L->new_2D_curve(C,true);

    w.show();
    return App.exec();
}
