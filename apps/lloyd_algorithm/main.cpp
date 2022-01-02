#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;

scalar R = 2;
//ConvexPolygon source = convexPrimitive::Circle(R,30);
ConvexPolygon source = convexPrimitive::Square(2*R).subdivide(4);
uint N = 60;

cloud lloyd_algorithm(cloud X,const ConvexPolygon& source){
    for (uint iter = 0;iter<15;iter++){
        Diagram V = Voronoi(X,R);
        for (uint i = 0;i<N;i++){
            auto C = ConvexPolygonIntersection(V.getCell(i),source);
            if (C.nbVertices() > 2)
                X[i] = C.Centroid();
        }
    }
    return X;
}

void PlotCentroids(const Diagram& V,PlotLayer* L){
    cloud X;
    for (uint i = 0;i<N;i++){
        const auto& C = ConvexPolygonIntersection(V.getCell(i),source);
        L->new_2D_curve(C.getIndexedCyclicPoints());
        X.add_point(C.Centroid());
    }
    L->new_point_cloud(X);
}

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    range pad = {-R-1,R+1};
    PlotLayer* L = F->add_grid_layer(pad,pad,false);

    auto Y = algo::stat::random_var::sample_uniform_in_square(2,R/sqrt(2),N);
    for (const auto& x : source.getIndexedPoints())
        Y.add_point(x);
    auto LL = lloyd_algorithm(Y);
    //L->new_point_cloud(Y);
    //L->new_point_cloud(LL);
    //auto V = Voronoi(LL,R);
    //L->addPlot<DiagramPlotter>(V);
    L->new_2D_curve(source.getIndexedCyclicPoints());
    //PlotCentroids(V,L);
    auto M = mesh_generation::BowyerWatson(LL);
    T->add_frame()->add_grid_layer(pad,pad,false)->addPlot<Mesh2DDisplayer>(M);

    w.show();
    return App.exec();
}
