#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;

int main(int argc, char *argv[])
{
    uint N = 40;
    auto X = algo::stat::random_var::sample_uniform_in_square(2,3,N);
    scalar r = 6;
    range R(-r,r);
    vec psi = algo::stat::random_var::sample_uniform_in_square(N,0,1)[0];

    scalar dw = 3;
    Diagram V = Voronoi(X,dw);

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    auto W = 300;
    auto M = algo::calculus::build_range_mapper({0,W},R);
    auto L = T->add_frame()->add_grid_layer(R,R,false);
    //L->new_figure_from_texturing(W,W,voronoi_approx);
    //auto D = mesh_generation::Delaunay(X);

    /*
    updateRoutine diagUpdate = [&X,dw,&V,N](scalar val) {
        vec psi(N);
        psi(0) = val;
        V = Laguerre(X,psi,dw);
    };
    w.add_mutable_scalar_by_cursor({-2,2},"PSI_0",40,diagUpdate);
    */
    L->new_point_cloud(X);
    L->addPlot<DiagramPlotter>(V);
    //L->addPlot<Mesh2DDisplayer>(D);

    w.show();
    return App.exec();
}

/*

    updateRoutine diagUpdate = [&X,dw,&V,N](scalar val) {
        vec psi(N);
        psi(0) = val;
        V = Laguerre(X,psi,dw);
    };
    w.add_mutable_scalar_by_cursor({-2,2},"PSI_0",40,diagUpdate);

    auto Verts = G.getIndexedVertices();
    auto voronoi_approx = [M,&Verts,&X,W,&psi] (uint i,uint j){
        auto x = vec({M(i),M(W-j)});
        auto C = *std::min_element(Verts.begin(),Verts.end(),[&X,x,&psi] (int a,int b){
            return 0.5*X[a].distance2(x) + psi(a) < 0.5*X[b].distance2(x) + psi(b);
        });
        return CNC_COLORS[C%10];
    };

    */
