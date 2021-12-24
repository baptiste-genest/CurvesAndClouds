#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
scalar R = 2;
scalar N = 20;

void RuppertChow(Mesh2& M){
    using trig = std::pair<face,circum_data>;
    static auto cmp = [] (const trig& a,const trig& b){
        return a.second.second < b.second.second;
    };
    auto C = M.getContext();
    std::priority_queue<trig,std::vector<trig>,decltype (cmp)> Q(cmp);
    for (const auto& f : M.faces())
        Q.push({f,(*C).circumCenterRadius(f)});
    for (int iter = 0;iter < 50;iter++){
        auto top = Q.top();Q.pop();
        auto N = M.insertVertexInFace(top.first,top.second.first);
        for (const auto& f : N)
            Q.push({f,(*C).circumCenterRadius(f)});
    }
}

Mesh2 RuppertChow(cloud X){
    uint N = 40;
    scalar threshold = 0.1;
    for (uint k = 0;k<N;k++){
        auto D = mesh_generation::Delaunay(X);
        const GeometricContext& C = *D.getContext();
        auto F = D.faces();
        auto bad = *std::max_element(F.begin(),F.end(),[C](const face& f1,const face& f2){
            return C.circumRadius(f1) < C.circumRadius(f2);
        });
        auto cc = C.circumCenter(bad);
        if (cc.norm_inf() > R)
            cc *= R/cc.norm_inf();
        X.add_point(cc);
        if (k == N-1)
            return D;
    }
}



int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);
    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    range ER = {-(R+1),R+1};
    PlotLayer* L = F->add_grid_layer(ER,ER,false);


    auto Y = algo::stat::random_var::sample_uniform_in_square(2,R,10);
    /*
    auto Tri = mesh_generation::Delaunay(Y);
    auto f = *Tri.faces().begin();
    Tri.insertVertexInFace(f,(*Tri.getContext()).circumCenter(f));
    */

    /*
    auto X = algo::stat::random_var::sample_uniform_in_square(2,R,N);
    auto D = mesh_generation::Delaunay(X);
    auto DR = mesh_generation::Delaunay(X);
    RuppertChow(DR);
    L->addPlot<Mesh2DDisplayer>(D);
    L->addPlot<Mesh2DDisplayer>(DR);
    */
    auto RC = RuppertChow(Y);
    L->addPlot<Mesh2DDisplayer>(RC);


    w.show();
    return App.exec();
}
