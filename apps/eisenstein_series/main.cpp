#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::symbolic;

cscalar lambert(const cscalar& q,uint a){
    if (std::abs(q) > 1.)
        std::cerr << "won't converge" << std::endl;

    cscalar sum = 0;
    auto qn = q;
    static constexpr uint N = 500;
    for (uint n = 1;n<N;n++){
        sum += std::pow(n,a)*qn/(1.-qn);
        qn = qn*q;
    }
    return sum;
}

scalar f(scalar x,scalar a,scalar b){
    return x*x*a + x*x*x*b -1.;
}
scalar df(scalar x,scalar a,scalar b){
    return 2.*x*a + 3.*x*x*b;
}

scalar newton(scalar a,scalar b){
    scalar x = 1.;
    while (std::abs(f(x,a,b)) > 1e-10)
        x -= f(x,a,b)/df(x,a,b);
    return std::pow(x,-0.25);
}

cscalar g2(const cscalar& tau){
    static const scalar factor = 60.*std::pow(M_PI,4)/45;
    cscalar q = std::exp(cscalar(0,2)*M_PI*tau);
    return factor*(1.+240.*lambert(q,3));
}

cscalar g3(const cscalar& tau){
    static const scalar factor = 140.*2*std::pow(M_PI,6)/945;
    cscalar q = std::exp(cscalar(0,2)*M_PI*tau);
    return factor*(1.-504.*lambert(q,5));
}

std::vector<cscalar> img_by_matrix(const mat& A){
    cscalar z(A(0,0),A(0,1)),w(A(1,0),A(1,1));
    return {w/z,z};
}

mat pow(const mat& A,scalar t){
    mat D(2);
    D(0,0) = std::pow(A(0,0),t);
    D(1,1) = std::pow(A(1,1),t);
    return D;
}

vec ctov(const cscalar& z1,const cscalar& z2){
    return vec({z1.real(),z1.imag(),z2.real(),z2.imag()});
}

vec stereoproj(const cscalar& z1,const cscalar& z2,scalar pole = 0.87){
    auto f = 1./(pole-z2.imag());
    return linear_utils::vec3(z1.real()*f,z1.imag()*f,z2.real()*f);
}

struct stereopole{
    mat P,Pi;
    stereopole() {
        std::vector<vec> vb = algo::stat::random_var::sample_vector_on_unit_sphere(4,4);
        vb = chaskal::gram_schmidt(vb);
        P = chaskal::build_basis(vb);
        Pi = P.transpose();
    }
    mat getMat(std::array<scalar,2> th) const {
        mat R(4);
        for (uint j = 0;j<2;j++){
            R(2*j,2*j) = cos(th[j]);
            R(2*j+1,2*j) = -sin(th[j]);
            R(2*j,2*j+1) = sin(th[j]);
            R(2*j+1,2*j+1) = cos(th[j]);
        }
        return P*R*Pi;
    }
};

vec stereoproj(vec X,scalar th1,scalar th2,scalar pole = 0.87){
    static stereopole SP;
    X = SP.getMat({th1,th2})*X;
    auto f = 1./(X(3)-pole);
    return linear_utils::vec3(X)*f;
}

cscalar brut_eisenstein(const mat& A,int k){
    int N = 20;
    cscalar s = 0;
    for (int m = -N;m<=N;m++)
        for (int n = -N;n<=N;n++)
            if (m!=n){
                vec zv = A*linear_utils::vec2(m,n);
                cscalar z(zv(0),zv(1));
                s += std::pow(z,-2*k);
            }
    return s;
}

scalar m2(const cscalar& z){
    auto mz = std::abs(z);
    return mz*mz;
}

mat genMat(uint length){
    mat U(2,2,{1,1,0,1.});
    mat V(2,2,{1,0,1,1.});
    mat A(2,2,{1,0,0,1.});
    std::string prod;
    for (uint n = 0;n<length;n++)
        if (rand()%3 == 0){
            prod += "U";
            A = A * U;
        }
        else {
            prod += "V";
            A = A*V;

        }
    std::cout << "MAT GENERATED: " << prod << std::endl;
    return A;
}

Variable Z[4];
ValuationSystem ValuateVector(const vec& z){
    ValuationSystem V = {Z[0] == z(0),
                         Z[1] == z(1),
                         Z[2] == z(2),
                         Z[3] == z(3)};
    return V;
}

cloud generate_trefoil_knot()
{
    cloud trefoil;
    cscalar I(0.,1.);
    const auto& a = Z[0];
    const auto& b = Z[1];
    const auto& c = Z[2];
    const auto& d = Z[3];
    Expression knot = pow(a+b*I,3)-27.*pow(c+d*I,2);

    svec G(4);
    for (uint j = 0;j<4;j++)
        G(j) = knot.differentiate(Z[j]);

    svec grads[3] = {svec(4),svec(4),svec(4)};
    for (uint j= 0;j<4;j++){
        grads[0](j) = Re(G(j));
        grads[1](j) = Im(G(j));
        grads[2](j) = Z[j];
    }
    /*
    std::cout << grads[0].print() << std::endl;
    std::cout << grads[1].print() << std::endl;
    std::cout << grads[2].print() << std::endl;
    */
    auto X = ctov(std::polar(1.,M_PI/3),I*std::pow(27.,-0.5)).normalize();
    scalar dt = 5e-4;
    trefoil.add_point(X);
    std::vector<vec> grad_eval(4);
    auto EX = ValuateVector(X);
    for (uint j = 0;j<3;j++)
        grad_eval[j] = grads[j].evaluate_real(EX);
    grad_eval[3] = algo::stat::random_var::random_vec_in_sphere(1.,4);
        //std::cout << P(X) << ' ' << X.norm() << std::endl;
    X += chaskal::gram_schmidt(grad_eval).back()*dt;
    trefoil.add_point(X);
    for (uint i = 0;i<50000;i++){
        EX = ValuateVector(X);
        for (uint j = 0;j<3;j++)
            grad_eval[j] = grads[j].evaluate_real(EX);
        grad_eval[3] = trefoil[i+1]-trefoil[i];
        X += chaskal::gram_schmidt(grad_eval).back()*dt;
        //std::cout << P(X) << ' ' << X.norm() << std::endl;
        trefoil.add_point(X);
        if (X.distance(trefoil[0]) < 0.01 && i > 1000)
            break;
    }
    std::cout << knot.evaluate(ValuateVector(X)) << ' ' << X.norm() << std::endl;
    return trefoil;
}

mat Rot(scalar th){
    mat R(3);
    R(0,0) = cos(th);
    R(2,0) = sin(th);
    R(1,1) = 1;
    R(0,2) = -sin(th);
    R(2,2) = cos(th);
    return R;
}
int main(int argc, char *argv[])
{
    srand(time(NULL));
    /*
    generate_trefoil_knot();
    return 0;
    */

    mat A= genMat(6);
    //mat A(2,2,{1,1,2,3});
    auto ep = algo::get_2x2_eigenpaires(A);
    auto P = chaskal::build_basis(ep);
    auto Pi = algo::invert22(P);
    auto D = algo::diag(ep);
    std::cout << P*D*Pi << std::endl;
    std::cout << D << std::endl;

    auto period = std::max(std::abs(std::log(D(0,0))),std::abs(std::log(D(1,1))));
    std::cout << period << std::endl;
    auto ls = algo::calculus::get_lin_space(0,period,2000);
    cloud knot;
    for (const auto& t : ls){
        auto phi = P*algo::diag({std::exp(t),std::exp(-t)})*Pi;
        auto phiB = img_by_matrix(phi);

        const cscalar& tau = phiB[0];
        const cscalar& z= phiB[1];

        auto G2 = g2(tau)/std::pow(z,4);
        auto G3 = g3(tau)/std::pow(z,6);

        auto k = newton(m2(G2),m2(G3));
        G2 = std::pow(k,-4)*G2;
        G3 = std::pow(k,-6)*G3;
        auto X = ctov(G2,G3);
        knot.add_point(X);
    }

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 3;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);
    auto E = L->add_euclidean_plane(r,r);

    scalar eps = 0.001;
    auto trefoil = generate_trefoil_knot().subsample(eps);
    knot = knot.subsample(eps);

    auto th = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta");
    auto th2 = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta2");
    auto th3 = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta3");
    auto zoom = w.add_mutable_scalar_by_cursor({0,4},"zoom");
    auto pole = w.add_mutable_scalar_by_cursor({-4,4},"stereo pole");

    auto Pr = [th,th2,th3,pole,zoom] (const vec& y){
        vec x = Rot(th)*stereoproj(y,th2,th3,pole)*zoom;
        return vec({x(0),x(1)});
    };

    for (const auto& x : knot.points){
        E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        })->set_color(QColorConstants::Red);
    }
    for (const auto& x : trefoil.points)
        E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        });
    for (const auto& angle : algo::calculus::get_lin_space(0,2*M_PI,100)){
        vec x(4);
        x(0) = cos(angle);
        x(1) = sin(angle);
        E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        })->set_color(QColorConstants::Green);
    }
    for (const auto& angle : algo::calculus::get_lin_space(0,2*M_PI,100)){
        vec x(4);
        x(2) = cos(angle);
        x(3) = sin(angle);
        E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        })->set_color(QColorConstants::Blue);
    }

    w.show();
    return App.exec();
}
