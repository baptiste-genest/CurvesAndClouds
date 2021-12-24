#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;

int N = 60;
scalar R = 3;
ConvexPolygon back = convexPrimitive::Rectangle(2*R,2*R,vec({R,0.}));
const scalar t = 0.1;

mat inverse_distance_matrix(const cloud& X){
    mat D(N);
    for (int i = 0;i<N;i++)
        for (int j = i+1;j<N;j++)
            if (i != j)
                D(i,j) = 1./X[i].distance(X[j]);
    D += D.transpose();
    return D;
}

mat Hessian(const Diagram& L,const mat& inverse_dist,const ConvexPolygon& source){
    mat H(N);
    auto context = L.getContext();
    for (int i = 0;i<N;i++){
        for (int j = i+1;j<N;j++){
            auto C = L.commonEdge(i,j);
            if (C != NULL_EDGE){
                auto S = L.polygonEdge(C);
                H(i,j) = std::min((ConvexPolygonIntersection(source,S).segLength()*(1-t)+t*ConvexPolygonIntersection(back,S).segLength())*inverse_dist(i,j),1e-8);
                //H(i,j) = std::min(ConvexPolygonIntersection(source,S).segLength(),S.segLength()*1e-8)*inverse_dist(i,j);
            }
        }
    }
    H += H.transpose();
    for (int j = 0;j<N;j++){
        scalar S = 0;
        for (int i = 0;i<N;i++)
            S += H(i,j);
        H(j,j) = -S;
    }
    //std::cout << H <<std::endl;
    return H;
}


vec computeCellsArea(const Diagram& L,const ConvexPolygon& source){
    vec A(N);
    auto C = L.getContext();
    for (int i = 0;i<N;i++){
        A(i) = (1-t)*ConvexPolygonIntersection(source,L.getCell(i)).Area() + ConvexPolygonIntersection(back,L.getCell(i)).Area()*t;
    }
    return A;
}

vec ConjugateGradient(const mat& A,const vec& b,const vec& x0,scalar eps){
    vec r = b - A*x0,p=r,x = x0,Ap,pr;
    scalar alpha,beta;
    scalar err = 2*eps;
    scalar ibm = 1./b.norm_inf();
    int iter = 0;
    while(err > eps && iter < 100){
        iter++;
        if (err > 1e8)
            throw Cnc_error("Conjugate gradient did not converge");
        Ap = A*p;
        alpha = r.norm2() / (p.scalar_product(Ap));
        x = x + p*alpha;
        pr = r;
        r = r - Ap*alpha;
        beta = r.norm2()/(pr.norm2());
        p = r + p*beta;
        err = ((vec)(A*x - b)).norm_inf()*ibm;
        if (true)
            std::cout <<"conj grad error: " << err << std::endl;
    }
    return x;
}

mat adjacency(const Diagram& D){
    mat A(N);
    for (int i = 0;i<N;i++)
        for (int j = 0;j<N;j++)
            A(i,j) = (D.commonEdge(i,j) == NULL_EDGE) ? 0 : 1;
    return A;
}

vec PerformSemiDiscreteOptimalTransport(const cloud& X,const vec& goal,const ConvexPolygon& source){
    vec psi(N);
    Diagram L = Laguerre(X,psi,R);
    scalar step = 1;
    vec A(N),grad(N);
    mat D = inverse_distance_matrix(X),H,P;
    int iter = 0;
    do{
        A = computeCellsArea(L,source);
        grad = goal-A;
        H = Hessian(L,D,source);
        //P = preconditionners::Jacobi(H);
        //vec p = ConjugateGradient(P*H,-P*grad,psi,1e-20);
        vec p = ConjugateGradient(H,-grad,psi,1e-20);
        psi -= p*step;
        L = Laguerre(X,psi,R);
        std::cout << grad.norm_inf() << " " << goal(0)*0.01 << std::endl;
        iter++;
    } while (grad.norm_inf() > goal(0)*0.01 && iter < 50);
    return psi;
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication App(argc,argv);
    PlotWindow w; w.resize(1000,500);

    PlotTab* T = w.add_tab("Laguerre Diagram");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_grid_layer({-R,3*R},{-R,R},false);

    //auto X = stat::random_var::PoissonSamplingInSphere(1,0.05,2,100);
    auto X = algo::stat::random_var::sample_uniform_in_square(2,R,N);
    N = X.size();
    vec goal(N);
    scalar split_mass = 4*R*R/N;
    for (int i = 0;i<N;i++)
        goal(i) = split_mass;

    ConvexPolygon source = convexPrimitive::Square(2*R,vec({R,0.}));
    //ConvexPolygon source = convexPrimitive::Circle(R,20);
    //ConvexPolygon source = convexPrimitive::Square(2*R);
    //ConvexPolygon source = back;

    //auto V = Laguerre(X,psi,R);
    //L->addPlot<DiagramPlotter>(V);
    //auto D = inverse_distance_matrix(X);
    L->new_2D_curve(source.getIndexedCyclicPoints());

    auto grad = [X,goal,source] (const vec& psi){
        auto L = Laguerre(X,psi,R);
        return goal-computeCellsArea(L,source);
    };
    auto plan = algo::calculus::optimization::gradient_descent_adaptive_step(grad,vec(N),5,2,3000);
    //auto plan = PerformSemiDiscreteOptimalTransport(X,goal,source);

    auto transportPlan = Laguerre(X,vec(N),R);
    L->addPlot<DiagramPlotter>(transportPlan);
    updateRoutine up = [plan,&transportPlan,X](scalar t){
        transportPlan = Laguerre(X,plan*t,R);
    };
    w.add_mutable_scalar_by_cursor({0,1},"interpolation",50,up);

    w.show();
    return App.exec();
}

    /*
    T->add_frame()->add_grid_layer({-R,R},{-R,R},false)->addPlot<DiagramPlotter>(transportPlan);
    L->new_point_cloud(X);
    */
   // auto plan = PerformSemiDiscreteOptimalTransport(X,goal,source);
