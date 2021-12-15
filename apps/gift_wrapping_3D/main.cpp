#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;
using namespace cnc::algo::topology;

scalar slope(const vec& A,const vec& B){
    return (B(1)-A(1))/(B(0)-A(0));
}

face get_first_facet(const GeometricContext& G){
    auto V = G.getIndexedVertices();
    auto cmp1 = [&G](const vertex& a,const vertex& b){
        return G(a)(0) < G(b)(0);
    };
    auto LMit = std::min_element(V.begin(),V.end(),cmp1);
    auto LM = *LMit;
    std::iter_swap(LMit,V.begin());
    auto cmp2 = [&G,LM](const vertex& a,const vertex& b){
        if (slope(G(LM),G(a)) < slope(G(LM),G(b)))
            return true;
        return false;
    };
    auto TMit = std::max_element(V.begin()+1,V.end(),cmp2);
    auto TM = *TMit;
    std::iter_swap(TMit,V.begin()+1);
    auto x = G(LM), y = G(TM);
    auto cmp3 = [&G,x,y](const vertex& a,const vertex& b){
        vec na = algo::cross(y-x,G(a)-x).normalize();
        vec nb = algo::cross(y-x,G(b)-x).normalize();
        return na(0) < nb(0);
    };
    auto NM = *std::max_element(V.begin()+2,V.end(),cmp3);
    return {topology::edge{LM,TM},topology::edge{LM,NM},topology::edge{TM,NM}};
}

std::pair<faces,EdgeFaceConnectivityGraph> giftWrapping(const GeometricContext& G){
    auto V = G.getVertices();
    faces H;
    auto F1 = get_first_facet(G);
    H.insert(F1);
    EdgeFaceConnectivityGraph edges;
    std::stack<topology::edge> single_edges;
    for (const auto& e : F1){
        single_edges.push(e);
        edges[e].insert(F1);
    }
    int iter = 0;
    while (!single_edges.empty()){
        iter++;
        auto CE = single_edges.top();single_edges.pop();
        if (edges[CE].size() == 1){
            auto F = *edges[CE].begin();
            auto predicate = [&G,F,CE](const vertex& a,const vertex& b){
                if (belong(a,CE))
                    return false;
                if (belong(b,CE))
                    return true;
                return G.facet_angle(F,CE,a) < G.facet_angle(F,CE,b);
            };
            auto next = *std::min_element(V.begin(),V.end(),predicate);
            auto NF = assemble_face(CE,next);
            for (const auto& ne : NF){
                edges[ne].insert(NF);
                if (edges[ne].size() == 1)
                    single_edges.push(ne);
            }
            H.insert(NF);
        }
    }
    return {H,edges};
}

using triangle = std::pair<face,vec>;
using mesh = std::vector<triangle>;

mesh compute_orientation(const GeometricContext& G,const faces& H){
    auto C = G.getPoints();
    auto M = algo::stat::compute_mean(C);
    mesh NF;
    scalar dx = 0.001;
    for (const auto& F : H){
        auto V = get_indexed_vertices(F);
        auto P0  = G(V[0]);
        auto N = algo::cross(G(V[1]) - P0,G(V[2]) - P0).normalize();
        if ((P0 + N*dx).distance(M) < (P0 - N*dx).distance(M))
            N *= -1;
        NF.push_back({F,N});
    }
    return NF;
}

vec get_plane_dual(const GeometricContext& G,const triangle& T){
    auto P0 = G(get_indexed_vertices(T.first)[0]);
    auto N = T.second;
    scalar f = -0.5/N(2);
    return vec({N(0)*f,N(1)*f,0});
}

std::pair<GeometricContext,edges> getDualMesh(const GeometricContext& G,const mesh& H,EdgeFaceConnectivityGraph& C){
    cloud Cp;
    edges DE;
    std::map<face,int,faceComp> M;
    for (int i = 0;i<(int)H.size();i++){
        M[H[i].first] = i;
    }
    for (int i = 0;i<(int)H.size();i++){
        Cp.add_point(get_plane_dual(G,H[i]));
        for (auto e : H[i].first){
            std::vector<int> tmp;
            for (const auto& fs : C[e])
                if (M.find(fs) != M.end())
                    tmp.push_back(M[fs]);
            if (tmp.size() == 2)
                DE.insert({tmp[0],tmp[1]});
        }
    }
    return {GeometricContext(Cp),DE};
}

int main(int argc, char *argv[])
{
    auto X = algo::stat::random_var::sample_uniform_in_square(2,2,20);
    auto X3 = X.apply_to_copy([] (const vec& x){
        return vec({x(0),x(1),x.norm2()});
    });
    //X3[0](2) -= 0.2;
    GeometricContext G(X3);
    auto V = G.getVertices();

    auto HC = giftWrapping(G);
    const auto& H = HC.first;
    auto NF = compute_orientation(G,H);
    faces LH;
    mesh LHM;
    for (const triangle& nf : NF){
        if (nf.second(2)<0){
            LH.insert(nf.first);
            LHM.push_back(nf);
        }
    }

    auto dualMesh = getDualMesh(G,LHM,HC.second);
    const auto& D = dualMesh.first;
    const auto& DE = dualMesh.second;
    auto VD = D.getVertices();

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
    range R(-3,3);
    auto E = Layer->add_euclidean_plane(R,R);
    E->set_dynamic();

    {
    std::vector<euclid::Point*> P;
    for (auto v : V){
        auto x = G(v);
        P.push_back(E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        },4));
    }
    /*
    for (const auto& face : LH){
        for (const auto& e : face)
            E->add_object<euclid::Segment>(P[e[0]],P[e[1]]);
    }
    */
    }

    {
    std::vector<euclid::Point*> P;
    for (const auto& x : D.getPoints().points){
        P.push_back(E->add_object<euclid::Point>([Pr,x](){
            return Pr(x);
        },4));
        P.back()->set_color(QColorConstants::Red);
    }
    for (const auto& e : DE)
        E->add_object<euclid::Segment>(P[e[0]],P[e[1]]);
    }

    w.show();
    return App.exec();
}
