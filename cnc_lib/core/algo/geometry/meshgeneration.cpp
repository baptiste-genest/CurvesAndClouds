#include "meshgeneration.h"

cnc::algo::geometry::Mesh2 cnc::algo::geometry::mesh_generation::Delaunay(const cnc::cloud &G,bool proj2D)
{
    if (G.size() < 3)
        throw "Convex hull only for more than 2 points";
    if (G.size() == 3){
        GeometricContextRef C = std::make_shared<GeometricContext>(G);
        Mesh2 H(C);
        H.F.insert(topology::assemble_face({0,1},2));
        H.induce_from_faces();
        return H;
    }
    static auto parabola_proj = [] (const vec& x){
        return vec({x(0),x(1),x.norm2()});
    };
    auto X = G;
    X.apply(parabola_proj);
    GeometricContextRef C = std::make_shared<GeometricContext>(std::move(X));
    auto H = giftWrapping(C);
    static auto lower_hull = [] (const Mesh2& M,const topology::face& F){
        return M.getNormal(F)(2) < 0;
    };
    H.filterFaces(lower_hull);
    if (proj2D){
        static auto proj2D = [] (const vec& x){
            return vec({x(0),x(1)});
        };
        H.G.get()->points.apply(proj2D);
        H.N.clear();
    }
    return H;
}

cnc::algo::geometry::MeshRef cnc::algo::geometry::mesh_generation::Refinement(const cloud& B, const ShapePredicate& P, cnc::scalar h){
    auto MR = std::make_shared<Mesh2>(BowyerWatson(B));
    auto& M = *MR;
    M.filterFaces([P](const Mesh2& M,const face& f){
        return P(M.G->midPoint(f));
    });
    M.filterDeadVertices();

    M.computeConnectivity();

    scalar target_edge_length = h*std::sqrt(3);
    scalar low = target_edge_length*4./5.;
    scalar high = target_edge_length*4./3.;

    for (uint i = 0;i<10;i++){
        split_long_edges(M,high);
        collapse_short_edges(M,low,high);
        /*
        equalize_valences(M);
        relaxation(M);
        */
    }
    //MR->filterDeadVertices();
    /*
    try {
    for (uint i = 0;i<10;i++){
        split_long_edges(M,high);
        collapse_short_edges(M,low,high);
        equalize_valences(M);
        relaxation(M);
    }
    MR->filterDeadVertices();
    }  catch (Cnc_error&) {
    }
    */

    return MR;
}

void cnc::algo::geometry::mesh_generation::split_long_edges(Mesh2 &M, scalar h)
{
    std::queue<topology::edge> to_split;
    const auto& C = *M.getContext();
    for (const auto& e : M.edges()){
        auto l = C.edgeLength(e);
        if (l > h)
            to_split.push(e);
    }
    while (!to_split.empty()){
        const auto& e = to_split.front();
        to_split.pop();
        auto m = M.edgeSplit(e);
        for (const auto& ne : M.VtoE[m]){
            auto l = C.edgeLength(ne);
            if (l > h){
                to_split.push(ne);
            }
        }
    }
}

void cnc::algo::geometry::mesh_generation::collapse_short_edges(Mesh2 &M, scalar low, scalar high)
{
    std::vector<topology::edge> to_collapse;
    const auto& C = *M.getContext();
    for (const auto& e : M.interiorEdges()){
        auto l = C.edgeLength(e);
        if (l < low)
            to_collapse.push_back(e);
    }
    for (const auto& e : to_collapse){
        if (M.E.find(e) == M.E.end() || M.isBoundaryEdge(e))
            continue;
        auto onering = M.getOneRingVertices(e[0]);
        bool collapse_ok = true;
        for (auto v : onering){
            if (C(e[1]).distance(C(v)) > high){
                collapse_ok = false;
                break;
            }
        }
        if (collapse_ok)
            M.edgeCollapseInto(e,e[0]);
    }

}

void cnc::algo::geometry::mesh_generation::equalize_valences(Mesh2 &M)
{
    const topology::edges E = M.interior_edges;
    //std::cout << M.EtoF.at({31,173}).size() << std::endl;
    //uint i = 0;
    for (const auto& e : E){
        if (M.shouldFlip(e))
            M.edgeFlip(e);
    }
}

void cnc::algo::geometry::mesh_generation::relaxation(Mesh2 &M)
{
    std::map<vertex,vec> npos;
    for (auto v : M.interior_vertices)
        npos[v] = M.oneRingBarycenter(v);
    auto& C = *M.getContext();
    for (const auto& v : npos)
        C(v.first) = v.second;
}

std::array<cnc::vec,3> cnc::algo::geometry::mesh_generation::super_triangle(const cnc::cloud &X)
{
    cnc::scalar eps = 5e-1;
    auto B = algo::get_min_max_range(X.points);
    const auto& xmin = B[0].first;
    const auto& xmax = B[0].second;
    const auto& ymin = B[1].first;
    const auto& ymax = B[1].second;
    return {
        vec({xmin-eps,ymin-eps}),
                vec({xmin+(xmax-xmin)*2 + 3*eps,ymin-eps}),
                vec({xmin-eps,ymin + 2*(ymax-ymin) + 3*eps}),
    };
}

bool cnc::algo::geometry::mesh_generation::inCircle(const cnc::vec &x, const cnc::vec &center, cnc::scalar r)
{
    return x.distance2(center) < r*r;
}

cnc::algo::geometry::Mesh2 cnc::algo::geometry::mesh_generation::BowyerWatson(const cloud& X){
    GeometricContextRef CR = std::make_shared<GeometricContext>(X);
    auto& C = *CR;
    auto V = C.getVertices();
    auto STv = super_triangle(X);
    auto v1 = C.add_vertex(STv[0]);
    auto v2 = C.add_vertex(STv[1]);
    auto v3 = C.add_vertex(STv[2]);
    auto ST = topology::assemble_face(v1,v2,v3);
    faces T = {ST};
    for (vertex v : V){
        insertVertexInDelaunay(C,T,v);
    }
    Mesh2 D(CR);
    auto stv = get_vertices(ST);
    for (const auto& f : T){
        bool insuper = false;
        for (const auto& v : stv)
            if (topology::belong(v,f)){
                insuper = true;
                break;
            }
        if (!insuper)
            D.F.insert(f);
    }
    D.induce_from_faces();
    return D;
}

void cnc::algo::geometry::mesh_generation::insertVertexInDelaunay(const cnc::algo::geometry::GeometricContext &C, cnc::algo::topology::faces &T, cnc::algo::topology::vertex v)
{
    std::map<topology::edge,int,edgeComp> edgeCount;
    edges englobing;
    faces to_delete;
    for (const auto& f : T){
        auto CC = C.circumCenterRadius(f);
        const auto& center = CC.first;
        const auto& radius = CC.second;
        if (C(v).distance(center) < radius + 1e-4){
            for (const auto& e : f){
                englobing.insert(e);
                edgeCount[e]++;
            }
            to_delete.insert(f);
        }
    }
    for (const auto& f : to_delete)
        T.erase(f);
    for (const auto& e : englobing){
        if (edgeCount[e] == 1){
            auto nT = assemble_face(e,v);
            T.insert(nT);
        }
    }
}
/*
void cnc::algo::geometry::mesh_generation::RBST::insert(const cnc::algo::topology::face &f, cnc::range r)
{
    insert_left(&head,f,r.first);
    insert_right(&head,f,r.second);
}

cnc::algo::topology::faces cnc::algo::geometry::mesh_generation::RBST::get(cnc::scalar x,const faces& T) const
{
    if (head == nullptr)
    return {};
    topology::faces f;
    get(head,f,x,T);
    return f;
}

void cnc::algo::geometry::mesh_generation::RBST::get(RBSTnode *n, cnc::algo::topology::faces &f, cnc::scalar x,const faces& T) const
{
    if (n != nullptr){
    if (n->v < x){
    if (n->upper)
    f.erase(n->f);
    else
    if (T.find(n->f) != T.end())
        f.insert(n->f);
    get(n->R,f,x,T);
    }
    else {
    if (!n->upper)
    f.erase(n->f);
    else{
    if (T.find(n->f) != T.end())
        f.insert(n->f);
    }
    get(n->L,f,x,T);
    }
    }
}

void cnc::algo::geometry::mesh_generation::RBST::insert_right(cnc::algo::geometry::mesh_generation::RBSTnode **n, const cnc::algo::topology::face &f, cnc::scalar x)
{
    if (*n == nullptr){
    *n = new RBSTnode;
    (*n)->f = f;
    (*n)->v = x;
    (*n)->upper = true;
    }
    else{
    if ((*n)->v < x)
    insert_right(&(*n)->R,f,x);
    else
    insert_right(&(*n)->L,f,x);
    }
}

void cnc::algo::geometry::mesh_generation::RBST::insert_left(cnc::algo::geometry::mesh_generation::RBSTnode **n, const cnc::algo::topology::face &f, cnc::scalar x)
{
    if (*n == nullptr){
    *n = new RBSTnode;
    (*n)->f = f;
    (*n)->v = x;
    (*n)->upper = false;
    }
    else{
    if ((*n)->v < x)
    insert_left(&(*n)->R,f,x);
    else
    insert_left(&(*n)->L,f,x);
    }
}
*/

cnc::algo::geometry::Mesh2 cnc::algo::geometry::mesh_generation::BowyerWatsonWIP(const cloud& X){
    GeometricContextRef CR = std::make_shared<GeometricContext>(X);
    auto& C = *CR;
    auto V = C.getVertices();
    auto STv = super_triangle(X);
    auto v1 = C.add_vertex(STv[0]);
    auto v2 = C.add_vertex(STv[1]);
    auto v3 = C.add_vertex(STv[2]);
    auto ST = topology::assemble_face(v1,v2,v3);
    faces T = {ST};
    RBST RX,RY;
    circumMap M;
    insertCircum(RX,RY,ST,C,M);
    for (vertex v : V){
        std::map<topology::edge,int,edgeComp> edgeCount;
        edges englobing;
        faces to_delete;
        for (const auto& f : nonDelaunayTriangles(C(v),RX,RY,M,T)){
            for (const auto& e : f){
                englobing.insert(e);
                edgeCount[e]++;
            }
            to_delete.insert(f);
        }
        for (const auto& f : to_delete)
            T.erase(f);
        for (const auto& e : englobing){
            if (edgeCount[e] == 1){
                auto nT = assemble_face(e,v);
                T.insert(nT);
                insertCircum(RX,RY,nT,C,M);
            }
        }
    }
    Mesh2 D(CR);
    auto stv = get_vertices(ST);
    for (const auto& f : T){
        bool insuper = false;
        for (const auto& v : stv)
            if (topology::belong(v,f)){
                insuper = true;
                break;
            }
        if (!insuper)
            D.F.insert(f);
    }
    D.induce_from_faces();
    return D;
}

cnc::algo::geometry::Mesh2 cnc::algo::geometry::mesh_generation::FromBoundaryMesh(cloud X, cnc::scalar treshold, const cnc::algo::geometry::ShapePredicate &P)
{
    bool done = false;
    while (!done){
        auto M = mesh_generation::BowyerWatson(X);
        const auto& C = *M.getContext();
        done = true;
        for (const auto& f : M.faces()){
            auto m = C.midPoint(f);
            scalar A = C.faceArea(f);
            if (A > treshold && P(m)){
                X.add_point(m);
                done = false;
            }
        }
    }
    auto final = mesh_generation::BowyerWatson(X);
    final.filterFaces([P](const Mesh2& M,const face& f){
        return P((*M.getContext()).midPoint(f));
    });
    std::cout << "Meshing complete, h = " << final.maxInscribedRadius() << std::endl;
    final.filterDeadVertices();
    return final;
}



void cnc::algo::geometry::mesh_generation::insertCircum(cnc::algo::geometry::mesh_generation::RBST &RX, cnc::algo::geometry::mesh_generation::RBST &RY, const cnc::algo::topology::face &f, const cnc::algo::geometry::GeometricContext &C, circumMap &M)
{
    auto CC = C.circumCenterRadius(f);
    const auto& center = CC.first;
    const auto& radius = CC.second;
    RX.insert({center(0)-radius,center(0)+radius},f);
    RY.insert({center(1)-radius,center(1)+radius},f);
    M[f] = CC;
}

cnc::algo::topology::faces cnc::algo::geometry::mesh_generation::nonDelaunayTriangles(const cnc::vec &x, const cnc::algo::geometry::mesh_generation::RBST &X, const cnc::algo::geometry::mesh_generation::RBST &Y,const cnc::algo::geometry::mesh_generation::circumMap& M,const faces& T)
{
    auto TX = X.checkOverlapValues(x(0));
    auto TY = Y.checkOverlapValues(x(1));
    faces I,F;
    std::set_intersection(TX.begin(),TX.end(),TY.begin(),TY.end(),std::inserter(I,I.begin()),faceComp());
    for (const auto& f : I){
        if (T.find(f) != T.end() && M.at(f).first.distance(x) > M.at(f).second)
            F.insert(f);
    }
    return F;
}
