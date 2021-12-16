#include "convex_hull.h"

cnc::algo::geometry::Mesh2 cnc::algo::geometry::giftWrapping(const GeometricContext &G)
{
    using namespace topology;
    auto V = G.getVertices();
    faces H;
    auto F1 = convex_hull_routines::get_first_facet(G);
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
    return Mesh2(G,H,edges,convex_hull_routines::compute_orientation(G,H));
}

cnc::algo::topology::face cnc::algo::geometry::convex_hull_routines::get_first_facet(const GeometricContext &G)
{
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

cnc::scalar cnc::algo::geometry::convex_hull_routines::slope(const vec &A, const vec &B)
{
    return (B(1)-A(1))/(B(0)-A(0));
}

cnc::algo::geometry::NormalMap cnc::algo::geometry::convex_hull_routines::compute_orientation(const GeometricContext &G, const topology::faces &H)
{
    auto C = G.getPoints();
    auto M = algo::stat::compute_mean(C);
    NormalMap NF;
    scalar dx = 0.001;
    for (const auto& F : H){
        auto V = get_indexed_vertices(F);
        auto P0  = G(V[0]);
        auto N = algo::cross(G(V[1]) - P0,G(V[2]) - P0).normalize();
        if ((P0 + N*dx).distance(M) < (P0 - N*dx).distance(M))
            N *= -1;
        NF[F] = N;
    }
    return NF;
}
