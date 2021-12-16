#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;

void plot_diagram(euclid::EuclideanPlane* E,const cloud& X,const Diagram& V){
    {
        std::vector<euclid::Point*> P;
        for (auto x : X.points){
            P.push_back(E->add_object<euclid::Point>([x](){
                return x;
            },4));
        }
    }
    {
        std::map<int,euclid::Point*> P;
        for (auto v : V.G.getVertices()){
            auto x = V.G(v);
            P[v] = E->add_object<euclid::Point>([x](){
                return x;
            },4);
            P[v]->set_color(QColorConstants::Red);
        }
        std::vector<topology::edge> C = V[0];
        for (const auto& e : V.edges){
            auto S = E->add_object<euclid::Segment>(P[e[0]],P[e[1]]);
            if (std::find(C.begin(),C.end(),e) != C.end())
                S->set_color(QColorConstants::Red);
        }

    }
}

int main(int argc, char *argv[])
{
    auto X = algo::stat::random_var::sample_uniform_in_square(2,2,5);

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");

    /*
    auto th = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta");
    auto phi = w.add_mutable_scalar_by_cursor({0,2*M_PI},"phi");
    auto Pr = [th,phi] (const vec& y){
        vec x = Rx(phi)*Ry(th)*vec({y(0),y(1),0});
        return vec({x(0),x(1)});
    };
    */

    range R(-20,20);
    {
        auto E = T->add_frame()->add_layer()->add_euclidean_plane(R,R);
        E->set_dynamic();
        Diagram V = Voronoi(X,R);
        plot_diagram(E,X,V);
    }
    {
        auto E = T->add_frame()->add_layer()->add_euclidean_plane(R,R);
        E->set_dynamic();
        vec psi(X.size());
        psi(0) = -0.1;
        Diagram V = Laguerre(X,psi,R);
        plot_diagram(E,X,V);
    }
    w.show();
    return App.exec();
}
