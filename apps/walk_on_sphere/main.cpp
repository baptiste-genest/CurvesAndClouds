#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;

vec WalkOnSphere(const Mesh2& M,const Polygon& B,const calculus::scalar_function& f,uint n,scalar eps){
    vec S(M.nbVertices());
    const auto& G = *M.getContext();
    for (const auto& b : M.getBoundaryVertices())
        S(b) = f(G(b));
    for (const auto& v : M.getInteriorVertices()){
        auto x0 = G(v);
        scalar val = 0;
        for (uint i = 0;i<n;i++){
            auto x = x0;
            scalar d = B.distanceTo(x);
            do {
                x += stat::random_var::sample_vector_on_unit_sphere(2,1)[0]*d;
                d = B.distanceTo(x);
            } while (d > eps);
            val += f(x);
        }
        S(v) = val/n;
    }
    return S;
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    scalar l = 1;
    range r{-l,l};
    PlotLayer* L = F->add_grid_layer(r,r,false);

    auto P = convexPrimitive::Square(1);
    auto M = std::make_shared<Mesh2>(mesh_generation::FromBoundaryMesh(P.subdivide(8).getPointCloud(),0.005,P.getShapePredicate()));
    M->computeConnectivity();
    auto v = WalkOnSphere(*M,P,[](const vec& x){
        return std::abs(x(0))>0.49;
    },200,1e-2);
    L->addPlot<Valued2DMeshDisplayer>(M,v);

    w.show();
    return App.exec();
}
