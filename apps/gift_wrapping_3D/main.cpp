#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;

scalar slope(const vec& A,const vec& B){
    return (B(1)-A(1))/(B(0)-A(0));
}

face get_first_facet(const GeometricContext& G){
    auto V = G.getVertices();
    auto cmp1 = [&G](const vertex& a,const vertex& b){
        return G(a)(0) < G(b)(0);
    };
    auto LM = *std::min_element(V.begin(),V.end(),cmp1);
    auto cmp2 = [&G,LM](const vertex& a,const vertex& b){
        if (LM == a)
            return true;
        if (LM == b)
            return false;
        if (slope(G(LM),G(a)) < slope(G(LM),G(b)))
            return true;
        return false;
    };
    auto TM = *std::max_element(V.begin(),V.end(),cmp2);
    auto x = G(LM), y = G(TM);
    auto cmp3 = [&G,x,y](const vertex& a,const vertex& b){
        vec na = algo::cross(y-x,G(a)-x).normalize();
        vec nb = algo::cross(y-x,G(b)-x).normalize();
        return na(0) < nb(0);
    };
    auto NM = *std::max_element(V.begin(),V.end(),cmp3);
    return {topology::edge{LM,TM},topology::edge{LM,NM},topology::edge{TM,NM}};
}

faces giftWrapping(const GeometricContext& G){
    auto V = G.getVertices();
    faces H;
    std::stack<face> incomplete_faces;
    incomplete_faces.push(get_first_facet(G));
    std::map<topology::edge,faces,edgeComp> edges;
    for (const auto& e : incomplete_faces.top())
        edges[e] = {incomplete_faces.top()};
    bool done = false;
    int iter = 0;
    while (!done && iter < 15){
        iter++;
        auto current = incomplete_faces.top();incomplete_faces.pop();
        for (const auto& e : current){
            printf("%i\n",(int)edges[e].size());
            if (edges[e].size() == 1){
                auto next = *std::min_element(V.begin(),V.end(),[&G,current,e](const vertex& a,const vertex& b){
                    if (belong(a,e))
                        return false;
                    if (belong(b,e))
                        return true;
                    return G.facet_angle(current,e,a) < G.facet_angle(current,e,b);});
                if (!belong(next,e)){
                    auto NF = assemble_face(e,next);
                    incomplete_faces.push(NF);
                    edges[{e[0],next}].insert(NF);
                    edges[{e[1],next}].insert(NF);
                    H.insert(NF);
                }
                else{
                    std::cout << "probs" << std::endl;
                }
            }
        }
        done = true;
        for (const auto& Fs : edges)
            if (Fs.second.size() != 2){
                done = false;
                break;
            }
    }
    return H;
}

int main(int argc, char *argv[])
{
    GeometricContext G(algo::stat::random_var::sample_uniform_in_square(3,1,20));
    auto V = G.getVertices();

    auto H = giftWrapping(G);

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    auto th = w.add_mutable_scalar_by_cursor({0,2*M_PI},"theta");
    auto phi = w.add_mutable_scalar_by_cursor({0,2*M_PI},"phi");

    PlotTab* T = w.add_tab("my first tab");

    auto Pr = [th,phi] (const vec& y){
        vec x = Rx(phi)*Ry(th)*y;
        return vec({x(0),x(1)});
    };
    PlotFrame* F= T->add_frame();
    PlotLayer* Layer = F->add_layer();
    range R(-2,2);
    auto E = Layer->add_euclidean_plane(R,R);
    E->set_dynamic();

    std::vector<euclid::Point*> P;

    for (auto v : V){
        auto x = G(v);
        P.push_back(E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        },4));
    }
    std::cout <<"HSIZE " << H.size() << std::endl;

    for (const auto& face : H){
        for (const auto& e : face)
            E->add_object<euclid::Segment>(P[e[0]],P[e[1]]);
    }

    w.show();
    return App.exec();
}
