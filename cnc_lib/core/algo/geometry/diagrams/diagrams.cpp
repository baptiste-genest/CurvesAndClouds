#include "diagrams.h"

const cnc::algo::topology::SimplicialPolygon &cnc::algo::geometry::Diagram::operator[](int i) const
{
    return cells[i];
}

cnc::algo::topology::edge cnc::algo::geometry::Diagram::commonEdge(int i, int j) const
{
    topology::edge E{i,j};
    if (CCG.find(E) == CCG.end())
        return topology::NULL_EDGE;
    return CCG.at(E);
}

void cnc::algo::geometry::Diagram::build_lower_hull_dual(const GeometricContext &C,scalar R)
{
    using namespace timeHandling;
    Chronometer CH;
    auto H = giftWrapping(C);
    CH.tick();
    static auto lower_hull = [] (const Mesh2& M,const topology::face& F){
        return M.getNormal(F)(2) < 0;
    };
    auto L = H.getSubMesh(lower_hull);
    CH.tick();
    cloud& P = G.getPoints();
    std::map<topology::face,int,topology::faceComp> M;//cells corners ids
    for (const topology::face& f : L.faces())
        M[f] = G.add_vertex(get_plane_dual(L.getNormal(f)));
    CH.tick();
    cells.resize(C.getVertices().size());
    std::vector<topology::edge> outer_edges;
    for (const topology::edge& e : L.edges()){
        auto facets = L.getIndexedFaces(e);
        topology::edge E;
        if (facets.empty())
            throw Cnc_error("Unconnected edge");
        if (facets.size() == 1){//boundary edge
            auto id = M[facets[0]];
            if (P[id].norm_inf() > R)
                continue;
            auto B = L.getBoundaryEdge(facets[0]);
            auto outN = geometry::get2DOutwardNormal(C(B[0]),C(B[1]),C(topology::get_other(facets[0],B)));
            auto clamped = G.add_vertex(geometry::smallest_positive_ray_square_intersection(P[id],outN,R));
            E = {clamped,id};
        }
        else{//interior edge
            int id1 = M[facets[0]];
            int id2 = M[facets[1]];
            bool id1inside = G(id1).norm_inf() < R;
            bool id2inside = G(id2).norm_inf() < R;
            if (!id1inside && !id2inside)
                continue;
            if (id1inside && id2inside)
                E = {id1,id2};
            else{
                auto in_out = id1inside ? topology::edge{id1,id2} : topology::edge{id2,id1};
                auto ge = G.get_vec_edge(in_out);
                auto clamped = G.add_vertex(geometry::smallest_positive_ray_square_intersection(P[in_out[0]],ge,R));
                E = {in_out[0],clamped};
            }
        }
        cells[e[0]].push_back(E);
        cells[e[1]].push_back(E);
        edges.insert(E);
        CCG[E] = e;
    }
    static auto max_proj = [] (const vec& x){
        if (std::abs(x(0)) > std::abs(x(1)))
            return vec({x(0),0.,0.});
        return vec({0.,x(1),0.});
    };
    std::vector<std::pair<int,topology::edge>> borders;
    int S = cells.size();
    for (int i = 0;i<S;i++){
        auto B = topology::completeCycle(cells[i]);
        if (B != topology::NULL_EDGE){
            auto v1 = max_proj(G(B[0]));
            auto v2 = max_proj(G(B[1]));
            auto corner = v1+v2;
            if (corner.norm() > 1.8*R){
                edges.insert(B);
                cells[i].push_back(B);
            }
            else {
                auto C = G.add_vertex(corner);
                for (int j = 0;j<2;j++){
                    topology::edge E{C,B[j]};
                    edges.insert(E);
                    cells[i].push_back(E);
                }
            }
        }
    }
}

cnc::algo::geometry::Diagram cnc::algo::geometry::Voronoi(const cloud &G,scalar R)
{
    static auto voronoi_proj = [] (const vec& x){
        return vec({x(0),x(1),x.norm2()});
    };
    auto X = G;
    X.apply(voronoi_proj);
    GeometricContext C(X);
    Diagram V;
    V.RV = R;
    V.build_lower_hull_dual(C,R);
    return V;
}

cnc::algo::geometry::Diagram cnc::algo::geometry::Laguerre(const cloud &X, const vec &psi,scalar R)
{
    auto G = X;
    for (uint i = 0;i<G.size();i++){
        auto x = G[i](0);
        auto y = G[i](1);
        G[i] = vec({x,y,x*x+y*y + psi(i)});
    }
    GeometricContext C(G);
    Diagram L;
    L.build_lower_hull_dual(C,R);
    L.RV = R;
    return L;
}
