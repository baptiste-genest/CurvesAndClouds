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
            auto clamped = G.add_vertex(P[id]+outN*4*R);
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
}

cnc::algo::geometry::DiagramRef cnc::algo::geometry::Voronoi(const cloud &C,scalar R)
{
    DiagramRef Vr = std::make_shared<Diagram>();
    auto& V = *Vr;
    V.RV = R;
    auto L = mesh_generation::BowyerWatson(C);
    L.computeConnectivity();
    GeometricContext& G = *V.getContext();
    cloud& P = G.getPoints();
    std::map<topology::face,int,topology::faceComp> M;//cells corners ids
    for (const topology::face& f : L.faces())
        M[f] = G.add_vertex(L.getContext().get()->circumCenter(f));
    /*
    for (auto& x : G.getPoints().points)
        x = vec({x(0),x(1)});
        */
    V.cells.resize(C.size(),ConvexPolygon(V.getContext()));
    for (const topology::edge& e : L.edges()){
        auto facets = L.getIndexedFaces(e);
        topology::edge E;
        if (facets.empty())
            throw Cnc_error("Unconnected edge");
        if (facets.size() == 1){//boundary edge
            auto id = M[facets[0]];
            auto outN = geometry::get2DOutwardNormal(C[e[0]],C[e[1]],C[topology::get_other(facets[0],e)]);
            auto clamped = G.add_vertex(P[id]+outN*4*R);
            E = {clamped,id};
        }
        else//interior edge
            E = {M[facets[0]],M[facets[1]]};
        V.edges.insert(E);
        V.CCG[e] = E;
        for (uint i = 0;i<2;i++)
            for (uint j = 0;j<2;j++)
                V.cells[e[i]].insert(E[j]);
    }
    return Vr;
}

cnc::algo::geometry::DiagramRef cnc::algo::geometry::Laguerre(const cloud &X, const vec &psi,scalar R)
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
    H.computeConnectivity();
    DiagramRef L = std::make_shared<Diagram>();
    L->buildDelaunayDual(H,X,R);
    L->RV = R;
    return L;
}

cnc::algo::geometry::Mesh2 cnc::algo::geometry::mesh_generation::LloydRelaxation(const cnc::algo::geometry::ConvexPolygon &B, uint N)
{
    auto BP = B.getPointCloud();
    auto r = algo::get_min_max_range(BP.points)[0].second;
    auto X = algo::stat::random_var::sample_uniform_in_square(2,r,N).filter(B.getShapePredicate());
    uint in = X.size();
    X += BP;
    auto L = algo::geometry::Voronoi(X,r);
    for (uint i = 0;i<20;i++){
        for (uint i = 0;i<in;i++)
            X[i] = ConvexPolygonIntersection(L->getCell(i),B).Centroid();
        if (i < 19)
            L = algo::geometry::Voronoi(X,r);
    }
    X += BP;
    return mesh_generation::BowyerWatson(X);
}

cnc::algo::geometry::Mesh2 cnc::algo::geometry::mesh_generation::LaplacianRelaxation(const std::vector<ConvexPolygon> &B, const ShapePredicate &P, uint N)
{
    cloud X;
    scalar lambda = 0.5;
    for (const auto& P : B)
        X += P.getPointCloud();
    auto r = algo::get_min_max_range(X.points)[0].second;
    int nb_boundary = X.size();
    X += algo::stat::random_var::sample_uniform_in_square(2,r,N).filter(P);
    int n = X.size();
    auto L = BowyerWatson(X);

    L.computeConnectivity();
    auto& C = *L.getContext();
    cloud M = stat::init_empty_cloud(n-nb_boundary,2);
    for (uint i = 0;i<10;i++){
        for (vertex k = nb_boundary;k<n;k++)
            M[k-nb_boundary] = C.midPoint(L.getOneRingVertices(k));
        for (vertex k = nb_boundary;k<n;k++)
            C(k) = algo::lerp(C(k),M[k-nb_boundary],lambda);
    }
    std::cout << "Meshing complete, h = " << L.maxInscribedRadius() << std::endl;
    return L;
}


