#include "diagrams.h"

cnc::algo::geometry::Diagram::Diagram()
{
    context = std::make_shared<GeometricContext>();
}

const cnc::algo::geometry::ConvexPolygon &cnc::algo::geometry::Diagram::operator[](int i) const
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

const cnc::algo::geometry::ConvexPolygon& cnc::algo::geometry::Diagram::getCell(int i) const
{
    return cells[i];
}

cnc::algo::geometry::GeometricContextRef cnc::algo::geometry::Diagram::getContext() const
{
    return context;
}

cnc::algo::geometry::ConvexPolygon cnc::algo::geometry::Diagram::polygonEdge(const cnc::algo::topology::edge &e) const
{
    ConvexPolygon S(context);
    S.insert(e[0]);
    S.insert(e[1]);
    return S;
}

void cnc::algo::geometry::Diagram::buildDelaunayDual(const Mesh2& L,const cloud& C, scalar R)
{
    using namespace timeHandling;
    GeometricContext& G = *context;
    cloud& P = G.getPoints();
    std::map<topology::face,int,topology::faceComp> M;//cells corners ids
    for (const topology::face& f : L.faces())
        M[f] = G.add_vertex(get_plane_dual(L.getNormal(f)));
    for (auto& x : G.getPoints().points)
        x = vec({x(0),x(1)});
    cells.resize(C.size(),ConvexPolygon(context));
    for (const topology::edge& e : L.edges()){
        auto facets = L.getIndexedFaces(e);
        topology::edge E;
        if (facets.empty())
            throw Cnc_error("Unconnected edge");
        if (facets.size() == 1){//boundary edge
            auto id = M[facets[0]];
            auto outN = geometry::get2DOutwardNormal(C[e[0]],C[e[1]],C[topology::get_other(facets[0],e)]);
            auto clamped = G.add_vertex(P[id]+outN*2*R);
            E = {clamped,id};
        }
        else//interior edge
            E = {M[facets[0]],M[facets[1]]};
        edges.insert(E);
        CCG[e] = E;
        for (uint i = 0;i<2;i++)
            for (uint j = 0;j<2;j++)
                cells[e[i]].insert(E[j]);
    }
    /*
    std::vector<topology::SimplicialPolygon> cellsEdges(C.size());
    for (const topology::edge& e : L.edges()){
        auto facets = L.getIndexedFaces(e);
        topology::edge E;
        if (facets.empty())
            throw Cnc_error("Unconnected edge");
        if (facets.size() == 1){//boundary edge
            auto id = M[facets[0]];
            if (P[id].norm_inf() > R){
                continue;
            }
            auto outN = geometry::get2DOutwardNormal(C[e[0]],C[e[1]],C[topology::get_other(facets[0],e)]);
            auto clamped = G.add_vertex(geometry::smallest_positive_ray_square_intersection(P[id],outN,R));
            E = {clamped,id};
        }
        else{//interior edge
            int id1 = M[facets[0]];
            int id2 = M[facets[1]];
            bool id1inside = G(id1).norm_inf() < R;
            bool id2inside = G(id2).norm_inf() < R;
            if (!id1inside && !id2inside){
                continue;
                //TODO EDGE ONLY TRAVERSING THE SQUARE

            }
            else if (id1inside && id2inside)
                E = {id1,id2};
            else{
                auto in_out = id1inside ? topology::edge{id1,id2} : topology::edge{id2,id1};
                auto ge = G.get_vec_edge(in_out);
                auto clamped = G.add_vertex(geometry::smallest_positive_ray_square_intersection(P[in_out[0]],ge,R));
                E = {in_out[0],clamped};
            }
        }
        //printf("insert %i (%f,%f) : %i (%f,%f)\n",E[0],G(E[0])(0),G(E[0])(1),E[1],G(E[1])(0),G(E[1])(1));
        cellsEdges[e[0]].push_back(E);
        cellsEdges[e[1]].push_back(E);
        edges.insert(E);
        CCG[e] = E;
    }
    static auto max_proj = [] (const vec& x){
        if (std::abs(x(0)) > std::abs(x(1)))
            return vec({x(0),0.});
        return vec({0.,x(1)});
    };
    int S = cellsEdges.size();
    for (int i = 0;i<S;i++){
        auto B = topology::completeCycle(cellsEdges[i]);
        if (B != topology::NULL_EDGE){
            auto v1 = max_proj(G(B[0]));
            auto v2 = max_proj(G(B[1]));
            auto corner = v1+v2;
            if (corner.norm() > 1.8*R){
                edges.insert(B);
                cellsEdges[i].push_back(B);
            }
            else {
                auto C = G.add_vertex(corner);
                for (int j = 0;j<2;j++){
                    topology::edge E{C,B[j]};
                    edges.insert(E);
                    cellsEdges[i].push_back(E);
                }
            }
        }
    }
    cells.resize(cellsEdges.size(),ConvexPolygon(context));
    for (uint i = 0;i<cellsEdges.size();i++)
        for (vertex v : topology::get_vertices(cellsEdges[i]))
            cells[i].insert(v);
    */

}

cnc::algo::geometry::Diagram cnc::algo::geometry::Voronoi(const cloud &C,scalar R)
{
    Diagram V;
    V.RV = R;
    V.buildDelaunayDual(mesh_generation::Delaunay(C,false),C,R);
    return V;
}

cnc::algo::geometry::Diagram cnc::algo::geometry::Laguerre(const cloud &X, const vec &psi,scalar R)
{
    cloud G = X;
    for (uint i = 0;i<G.size();i++){
        auto x = G[i](0);
        auto y = G[i](1);
        G[i] = vec({x,y,x*x+y*y + psi(i)});
    }
    auto H = giftWrapping(G);
    static auto lower_hull = [] (const Mesh2& M,const topology::face& F){
        return M.getNormal(F)(2) < 0;
    };
    H.filterFaces(lower_hull);
    Diagram L;
    L.buildDelaunayDual(H,X,R);
    L.RV = R;
    return L;
}
