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

void cnc::algo::geometry::Diagram::build_lower_hull_dual(const GeometricContext &C,range R)
{
    auto H = giftWrapping(C);
    static auto lower_hull = [] (const Mesh2& M,const topology::face& F){
        return M.getNormal(F)(2) < 0;
    };
    auto L = H.getSubMesh(lower_hull);
    cloud& P = G.getPoints();
    std::map<topology::face,int,topology::faceComp> M;
    uint i = 0;
    for (const auto& f : L.faces()){
        P.add_point(get_plane_dual(L.getNormal(f)));
        M[f] = i;
        i++;
    }
    static auto outer_normal = [] (const vec& v){
        return vec({v(0)*10,v(1)*10,0});
    };
    for (const auto& b : L.boundaryEdges()){
        auto f = *L.getFaces(b).begin();
        auto fo = H.get_opposite(f,b);
        auto N = H.getNormal(fo);
        P.add_point(outer_normal(N));
        M[fo] = i;
        i++;
    }
    cells.resize(C.getVertices().size());
    for (const auto& e : L.edges()){
        auto fs = H.getFaces(e).begin();
        int id1 = M[*fs];
        ++fs;
        int id2 = M[*fs];
        topology::edge E{id1,id2};
        cells[e[0]].push_back(E);
        cells[e[1]].push_back(E);
        edges.insert(E);
        CCG[E] = e;
    }
}

cnc::algo::geometry::Diagram cnc::algo::geometry::Voronoi(const cloud &G,range R)
{
    static auto voronoi_proj = [] (const vec& x){
        return vec({x(0),x(1),x.norm2()});
    };
    auto X = G;
    X.apply(voronoi_proj);
    auto MH = (*std::max_element(X.points.begin(),X.points.end(),[](const vec& a,const vec& b){
        return a(2) < b(2);
    }))(2);
    X.add_point(vec({0.,0.,MH+1}));
    GeometricContext C(X);
    Diagram V;
    V.build_lower_hull_dual(C,R);
    return V;
}

cnc::algo::geometry::Diagram cnc::algo::geometry::Laguerre(const cloud &X, const vec &psi,range R)
{
    auto G = X;
    for (uint i = 0;i<G.size();i++){
        auto x = G[i](0);
        auto y = G[i](1);
        G[i] = vec({x,y,x*x+y*y + psi(i)});
    }
    auto MH = (*std::max_element(G.points.begin(),G.points.end(),[](const vec& a,const vec& b){
        return a(2) < b(2);
    }))(2);
    G.add_point(vec({0.,0.,MH+1}));
    GeometricContext C(G);
    Diagram L;
    L.build_lower_hull_dual(C,R);
    return L;
}
