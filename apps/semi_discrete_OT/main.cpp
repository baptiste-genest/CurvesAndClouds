#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;

int N = 40;
scalar R = 3;
ConvexPolygon back = convexPrimitive::Square(2*R);
using ConvexUnion = std::vector<ConvexPolygon>;
//ConvexPolygon back = convexPrimitive::Rectangle(2*R,2*R,vec({R,0.}));
//const scalar t = 0.1;

SMB inverse_distance_smatrix(const cloud& X){
    SMB D(N,true);
    for (int i = 0;i<N;i++)
        for (int j = i+1;j<N;j++)
            if (i != j)
                D(i,j) = 1./X[i].distance(X[j]);
    return D;
}


smat Hessian(const Diagram& L,const SMB& inverse_dist,const ConvexUnion& U){
    SMB H(N,true);
    auto context = L.getContext();
    for (int i = 0;i<N;i++){
        for (int j = i+1;j<N;j++){
            auto C = L.commonEdge(i,j);
            if (C != NULL_EDGE){
                auto S = L.polygonEdge(C);
                scalar l = 0;
                for (const auto& source : U)
                     l += ConvexPolygonIntersection(source,S).segLength()*inverse_dist(i,j);
                H(i,j) = l;
            }
        }
    }
    uint j = 0;
    scalar row_sum = 0;
    for (const auto& iv : H.getValues()){
        if (iv.first.second != j){
            if (std::abs(row_sum) > 1e-20)
                H(j,j) = -row_sum;
            row_sum = 0;
            j = iv.first.second;
        }
        row_sum += iv.second;
    }
    if (std::abs(row_sum) > 1e-20)
        H(j,j) = -row_sum;
    H.print();
    return H;
}


vec computeCellsArea(const Diagram& L,const ConvexUnion& U){
    vec A(N);
    auto C = L.getContext();
    for (int i = 0;i<N;i++){
        //A(i) = (1-t)*ConvexPolygonIntersection(source,L.getCell(i)).Area() + ConvexPolygonIntersection(back,L.getCell(i)).Area()*t;
        for (const auto& source : U)
            A(i) += ConvexPolygonIntersection(source,L.getCell(i)).Area();
    }
    return A;
}

vec ConjugateGradient(const smat& A,const vec& b,const vec& x0,scalar eps){
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
        if (false)
            std::cout <<"conj grad error: " << err << std::endl;
    }
    return x;
}

vec PerformSemiDiscreteOptimalTransport(const cloud& X,const vec& goal,const ConvexUnion& source){
    vec psi(N);
    Diagram L = Laguerre(X,psi,R);
    scalar step = 0.01;
    vec A(N),grad(N);
    SMB D = inverse_distance_smatrix(X);
    int iter = 0;
    do{
        A = computeCellsArea(L,source);
        grad = goal-A;
        smat H = Hessian(L,D,source);
        //H.print();
        //P = preconditionners::Jacobi(H);
        //vec p = ConjugateGradient(P*H,-P*grad,psi,1e-20);
        vec p = ConjugateGradient(H,-grad,psi,1e-20);
        psi -= p*step;
        L = Laguerre(X,psi,R);
        std::cout << grad.norm_inf() << " " << goal(0)*0.01 << std::endl;
        iter++;
    } while (grad.norm_inf() > goal(0)*0.05 && iter < 500);
    return psi;
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("Laguerre Diagram");
    PlotFrame* F= T->add_frame();

    //auto X = stat::random_var::PoissonSamplingInSphere(1,0.05,2,100);
    auto X = algo::stat::random_var::sample_uniform_in_square(2,R,N);
    //N = X.size();

    //ConvexPolygon source = convexPrimitive::Square(2*R,vec({R,0.}));
    //ConvexPolygon source = convexPrimitive::Circle(R,20);
    ConvexPolygon origin = convexPrimitive::Square(2*R);
    ConvexUnion source;
    source.push_back(convexPrimitive::Circle(R,20));
    //source.push_back(convexPrimitive::Square(2*R));
    /*
    source[0] = convexPrimitive::Rectangle(R,2*R,vec({-R,0.}));
    source[1] = convexPrimitive::Rectangle(R,2*R,vec({+R,0.}));
    */
    //ConvexPolygon source = back;


    PlotLayer* L = F->add_grid_layer({-R-1,R+1},{-R-1,R+1},false);
    auto V = Voronoi(X,R);
    L->addPlot<DiagramPlotter>(V);

    scalar tA = 0;
    for (const auto& s : source)
        tA += s.Area();
    vec goal = ones(N)*tA/(N);
    std::cout << goal << std::endl;

    //goal= computeCellsArea(V,{origin});
    /*
    scalar split_mass = 4*R*R/N;
    for (int i = 0;i<N;i++)
        goal(i) = split_mass;
        */

    /*
    for (int i = 0;i<N;i++){
        const auto& C = V.getCell(i);
        for (const auto& e : C.getIndexedCyclicEdges()){
            PlotLayer* L = F->add_grid_layer({-R-1,R+1},{-R-1,R+1},false);
            L->new_point_cloud(X,2,true);
            auto S = V.polygonEdge(e);
            auto I = ConvexPolygonIntersection(source,S);
            L->addPlot<DiagramPlotter>(V);
            if (I.nbVertices() >= 2){
                L->new_2D_curve(I.getIndexedCyclicPoints())->set_color(QColorConstants::Black);
            }
        }
    }
    */
    L->new_2D_curve(origin.getIndexedCyclicPoints());
    for (const auto& s : source)
        L->new_2D_curve(s.getIndexedCyclicPoints());
    //L->new_2D_curve(source.getIndexedCyclicPoints());

    auto grad = [X,goal,source] (const vec& psi){
        auto L = Laguerre(X,psi,R);
        return goal-computeCellsArea(L,source);
    };
    //auto plan = algo::calculus::optimization::gradient_descent_adaptive_step(grad,vec(N),5,2,3000);
    //auto plan = algo::calculus::optimization::gradient_descent_fixed_step(grad,vec(N),0.05,0.1,3000);
    auto plan = PerformSemiDiscreteOptimalTransport(X,goal,source);

    auto transportPlan = Laguerre(X,vec(N),R);
    L->addPlot<DiagramPlotter>(transportPlan);
    updateRoutine up = [plan,&transportPlan,X](scalar t){
        transportPlan = Laguerre(X,plan*t,R);
    };
    w.add_mutable_scalar_by_cursor({0,1},"interpolation",50,up);

    w.show();
    return App.exec();
}
