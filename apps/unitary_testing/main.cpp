#include <curvesandcloudCore.h>
using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::calculus;
using namespace cnc::algo::stat;
using namespace cnc::algo::stat::random_var;
using namespace cnc::algo::vector_calculus;

#include <algorithm>
#include <string>

bool test(std::string str, bool rslt){
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    if (!rslt)
        std::cout << "	FAIL : " << str << std::endl;
    else
        std::cout << "	SUCCESS : " << str << std::endl;
    return rslt;
}

bool eql(float a,float b,float eps){
    return std::abs(a-b) < eps;
}
bool eql(const vec& a,const vec& b,float eps){
    return (a-b).norm() < eps;
}
bool eql(int a,int b,float){
    return a == b;
}
bool eql(uint a,uint b,float){
    return a == b;
}

template<class T>
bool eq(const T& a,const T& b,float eps = 1e-5){
    if (eql(a,b,eps))
        return true;
    std::cout << "expected :" << std::endl << b << std::endl << "given :" << std::endl << a << std::endl;
    return false;
}

void declare( std::string str) {
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    std::cout << "START TEST " << str << std::endl;
}

int main()
{
    {
        declare("lin space with given dx");
        auto L = get_lin_space(0,1,0.01f);
        test("lower bound",eq(L[0],0.f));
        test("upper bound",eq(L.back(),1.f));
    }    {
        declare("lin space with fixed number of nodes");
        auto L = get_lin_space(0,1,10);
        test("lower bound",eq(L[0],0.f));
        test("upper bound",eq(L.back(),1.f));
        test("size",eq((int)L.size(),10));
    }    {
        declare("1d scalar linear interpolators");
        auto I = build_1D_linear_interpolator({0.f,1.f},{0.f,10.f});
        test("interpolation in range",eq(I(0.5f),5.f));
        test("interpolation out of range",eq(I(-1.f),0.f));
    }	{
        declare("range mapper");
        auto I = build_range_mapper({0.f,1.f},{0.f,10.f});
        test("mapping left bound ",eq(I(0.f),0.f));
        test("mapping right bound ",eq(I(1.f),10.f));
        test("interpolation in range",eq(I(0.5f),5.f));
        test("interpolation out of range",eq(I(-1.f),0.f));
    }   {
        declare("1D scalar function sampling");
        auto L = get_lin_space(0,2,10);
        auto S = sample([] (float x) {return x*x;},L);
        test("bound sampling ",eq(S.back(),4.f));
    }	{
        declare("scalar 2D laplacian");
        auto LF = build_2D_laplacian([] (float x,float y) { return std::cos(x*y); },1e-2);
        test("approximation quality 1e-5",eq(LF(1.f,1.f),-1.08004f));
    }	{
        using namespace integration_scheme;
        declare("integration schemes");
        auto X = get_lin_space(0,M_PI_2,100);
        auto f = [] (float x) {return std::sin(x);};
        test("left rectangle at 1e-2",eq(integrate(left_rectangle(f),X),1.f,1e-2));
        test("right rectangle at 1e-2",eq(integrate(right_rectangle(f),X),1.f,1e-2));
        test("mid point at 1e-4",eq(integrate(mid_point(f),X),1.f,1e-4));
        test("trapezoidal at 1e-4",eq(integrate(trapezoidal(f),X),1.f,1e-4));
        test("simpson at 1e-6",eq(integrate(simpson(f),X),1.f,1e-6));
    }	{
        declare("finite differences");
        auto f = [] (float x) {return std::sin(x);};
        auto df = [] (float x) {return std::cos(x);};
        auto d2f = [] (float x) {return -std::sin(x);};
        auto Df = build_derivative(f,1e-3);
        auto D2f = build_second_derivative(f,1e-2);
        float x = 1.2f;
        test("first order derivative",eq(Df(x),df(x),1e-3));
        test("second order derivative",eq(D2f(x),d2f(x),1e-3));
    }	{
        declare("Lagrange interpolation");
        auto X = get_lin_space(0,2,10);
        auto Y = sample([](float x) {return std::sin(x);},X);
        lagrange_interpolator LI(X,Y);
        bool ok = true;
        for (uint k = 0;k<10;k++)
            ok &= eq(LI(X[k]),Y[k]);
        test("interpolation at nodes",ok);
    }   {
        declare("vector calculus");
        auto V = [] (float x,float y){
            return vec({x,y});
        };
        auto S = [] (float x,float y){
            return x+y;
        };
        auto DV = vector_calculus::build_divergence(V);
        test("divergence at 1e-4",eq(DV(1,1)-2.f,0.f,1e-4));
        auto RV = vector_calculus::build_rot_norm(V);
        test("curl norm at 1e-4",eq(RV(1,1),0.f,1e-4));
        auto G = vector_calculus::build_gradient(S);
        test("gradient at 1e-3",eq(G(1,1),vec({1.f,1.f}),1e-3));

        auto z = [] (float x,float y){
            return cnc::vec({std::sin(4*y)+x,y+std::sin(4*x)});
        };
        range x = {-1,1};
        auto X = algo::calculus::get_lin_space(x,100);
        Vector_2D_array Z(X,X,z);
        test("discrete vector field sampling",eq(Z(0,0),z(-1,-1)));
        auto I = Z.get_interpolator(x,x);
        test("discrete vector field interpolation at 1e-3",eq(I(-0.9,-0.9),z(-0.9,-0.9),1e-3));
    }	{
        using namespace algo::differential_equations::ODE;
        declare("ODE");
        auto f = [] (const vec& x,float) { return x*(-1.f);};
        float dt = 1e-2;uint N = 500;float eps = 1e-2;
        vec x0({1.f,1.f}),expect(2);
        ODE_scheme ee = build_euler_explicit(f,dt);
        ODE_scheme rk2 = build_runge_kutta_2(f,dt);
        ODE_scheme rk4 = build_runge_kutta_4(f,dt);
        auto S1 = extract_space_steps(solve_ODE(ee,x0,0.f,dt,N));
        test("explicit euler with dt = 1e-2 : ",eq(S1.back(),expect,eps));
        auto S2 = extract_space_steps(solve_ODE(rk2,x0,0.f,dt,N));
        test("RK2 with dt = 1e-2 : ",eq(S2.back(),expect,eps));
        auto S3 = extract_space_steps(solve_ODE(rk4,x0,0.f,dt,N));
        test("RK4 with dt = 1e-2 : ",eq(S3.back(),expect,eps));
    }	{
        declare("optimization");
        using namespace algo::calculus::optimization;
        float goal = sqrt(2.f);
        auto f = [] (float x) {return x*x-2.f;};
        auto df = [] (float x) {return 2.f*x;};
        float rslt = newton_method_1D(f,df,1.f);
        test("1D newton method",eq(goal,rslt));

        auto g = [] (const vec& x) {return x.norm2();};
        vec x0({10.f,20.f,30.f}),expect(3);
        vec min = gradient_descent_fixed_step(g,x0,0.1f);
        test("fixed step gradient descent",eq(expect,min));
    }
}
