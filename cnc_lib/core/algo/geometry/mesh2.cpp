#include "mesh2.h"

cnc::algo::geometry::Mesh2::Mesh2(GeometricContextRef C, const topology::faces &Fa, const EdgeFaceConnectivityGraph &EF, const NormalMap &NM) : G(C),F(Fa),EtoF(EF),N(NM)
{
    induce_from_faces();
}

cnc::algo::geometry::Mesh2::Mesh2(cnc::algo::geometry::GeometricContextRef C, const cnc::algo::topology::faces &Fa):G(C),F(Fa)
{
    induce_from_faces();
}

cnc::algo::geometry::Mesh2::Mesh2(cnc::algo::geometry::Mesh2 &&O) : G(O.G)
{
    //std::cout << "mesh moved" << std::endl;
    V = std::move(O.V);
    E = std::move(O.E);
    F = std::move(O.F);
    boundary = std::move(O.boundary);
    interior_edges = std::move(O.interior_edges);
    EtoF = std::move(O.EtoF);
    N = std::move(O.N);
    interior_vertices = std::move(O.interior_vertices);
    boundary_vertices = std::move(O.boundary_vertices);
}

cnc::algo::geometry::Mesh2::Mesh2(const Mesh2 &other) : Mesh2(other.G,other.F)
{
}

cnc::vec cnc::algo::geometry::Mesh2::getNormal(const topology::face &F) const
{
    return N.at(F);
}

cnc::algo::geometry::Mesh2 cnc::algo::geometry::Mesh2::getSubMesh(const trianglePredicate &P) const
{
    Mesh2 S(G);
    for (const auto& f : F)
        if (P(*this,f)){
            S.F.insert(f);
            S.N[f] = N.at(f);
        }
    S.induce_from_faces();
    return S;
}

void cnc::algo::geometry::Mesh2::filterFaces(const cnc::algo::geometry::trianglePredicate &P)
{
    auto oldF = F;
    NormalMap newN;
    F.clear();
    //N.clear();
    for (const auto& f : oldF)
        if (P(*this,f)){
            F.insert(f);
            if (!N.empty())
                newN[f] = N.at(f);
        }
    N = std::move(newN);
    clear_connectivity();
    induce_from_faces();
}

const cnc::algo::topology::faces& cnc::algo::geometry::Mesh2::faces() const
{
    return F;
}

const cnc::algo::topology::vertices &cnc::algo::geometry::Mesh2::getVertices() const
{
    return V;
}

const cnc::algo::topology::faces &cnc::algo::geometry::Mesh2::getFaces(const topology::edge &e) const
{
    checkConnectivity();
    return EtoF.at(e);
}

std::vector<cnc::algo::topology::face> cnc::algo::geometry::Mesh2::getIndexedFaces(const cnc::algo::topology::edge &e) const
{
    std::vector<cnc::algo::topology::face> facets;
    for (const auto& f : getFaces(e))
        facets.push_back(f);
    return facets;
}

const cnc::algo::topology::edges &cnc::algo::geometry::Mesh2::edges() const
{
    return E;
}

const cnc::algo::topology::edges &cnc::algo::geometry::Mesh2::interiorEdges() const
{
    return interior_edges;
}

const cnc::algo::topology::edges &cnc::algo::geometry::Mesh2::boundaryEdges() const
{
    return boundary;
}

const cnc::algo::topology::edge &cnc::algo::geometry::Mesh2::getBoundaryEdge(const cnc::algo::topology::face &F) const
{
    for (const auto& e : F){
        if (EtoF.at(e).size() == 1)
            return e;
    }
    throw Cnc_error("no boundary edge in face");
}

int cnc::algo::geometry::Mesh2::nbVertices() const
{
    return V.size();
}

cnc::algo::topology::face cnc::algo::geometry::Mesh2::get_opposite(const topology::face &F, const topology::edge &e) const
{
    checkConnectivity();
    if (EtoF.at(e).size() == 1)
        throw Cnc_error("no opposite face");
    auto fs = EtoF.at(e).begin();
    if (topology::faceComp()(*fs,F))
        return *fs;
    fs++;
    return *fs;
}

cnc::vec cnc::algo::geometry::Mesh2::getVertexPosition(cnc::algo::topology::vertex v) const
{
    return (*G)(v);
}

std::array<cnc::algo::topology::face, 3> cnc::algo::geometry::Mesh2::insertVertexInFace(const cnc::algo::topology::face &f, const cnc::vec &x)
{
    F.erase(f);
    auto np = G.get()->add_vertex(x);
    std::array<cnc::algo::topology::face, 3> new_faces;
    for (int i = 0;i<3;i++){
        new_faces[i] = topology::assemble_face(f[i],np);
        insertFace(new_faces[i]);
    }
    return new_faces;
}

cnc::algo::geometry::GeometricContextRef cnc::algo::geometry::Mesh2::getContext()
{
    return G;
}

const cnc::algo::geometry::GeometricContextRef cnc::algo::geometry::Mesh2::getContext() const
{
    return G;
}

void cnc::algo::geometry::Mesh2::insertFace(const cnc::algo::topology::face &f)
{
    //compute_connectivity = false;
    F.insert(f);
    for (const auto& e : f){
        E.insert(e);
        for (auto v : e)
            V.insert(v);
    }
}

void cnc::algo::geometry::Mesh2::computeConnectivity()
{
    if (compute_connectivity)
        return;
    compute_connectivity = true;
    for (const auto& f : F)
        induce_face_connectivity(f);
    for (const auto& ef : EtoF)
        if (ef.second.size() == 1){
            boundary.insert(ef.first);
            for (vertex v : ef.first)
                boundary_vertices.insert(v);
        }
        else
            interior_edges.insert(ef.first);
    interior_vertices = V;
    for (auto v : boundary_vertices)
        interior_vertices.erase(v);
}

bool cnc::algo::geometry::Mesh2::isInteriorVertex(cnc::algo::topology::vertex x) const
{
    checkConnectivity();
    return interior_vertices.find(x) != interior_vertices.end();
}

bool cnc::algo::geometry::Mesh2::isBoundaryEdge(const cnc::algo::topology::edge &e) const
{
    checkConnectivity();
    if (boundary.find(e) != boundary.end())
        return true;
    return false;
}

bool cnc::algo::geometry::Mesh2::isBoundaryVertex(cnc::algo::topology::vertex v) const
{
    checkConnectivity();
    if (boundary_vertices.find(v) != boundary_vertices.end())
        return true;
    return false;
}

const cnc::algo::topology::face &cnc::algo::geometry::Mesh2::getOppositeFace(const cnc::algo::topology::face &f, const cnc::algo::topology::edge &e) const
{
    checkConnectivity();
    const auto& F = EtoF.at(e);
    if (F.size() <= 1)
        throw Cnc_error("No opposite edge");
    return *std::find_if_not(F.begin(),F.end(),[f] (const topology::face& o) { return f == o;} );
}

const cnc::algo::topology::faces &cnc::algo::geometry::Mesh2::getAdjacentFaces(const cnc::algo::topology::edge &e) const
{
    checkConnectivity();
    const auto& F = EtoF.at(e);
    return F;
}

const cnc::algo::topology::faces &cnc::algo::geometry::Mesh2::getOneRingFaces(cnc::algo::topology::vertex v) const
{
    checkConnectivity();
    const auto& F = VtoF.at(v);
    if (F.size() <= 1)
        throw Cnc_error("empty one ring");
    return F;

}

cnc::algo::topology::vertices cnc::algo::geometry::Mesh2::getOneRingVertices(vertex v) const
{
    checkConnectivity();
    topology::vertices V;
    for (const auto& e : VtoE.at(v))
        V.insert(topology::get_other(e,v));
    return V;
}

const cnc::algo::topology::edges& cnc::algo::geometry::Mesh2::getOneRingEdges(cnc::algo::topology::vertex v) const
{
    checkConnectivity();
    return VtoE.at(v);
}

cnc::algo::topology::edge cnc::algo::geometry::Mesh2::edgeFlip(const cnc::algo::topology::edge &edge)
{
    checkConnectivity();
    topology::edge NE;
    {
        const auto& FA = EtoF.at(edge);
        if (FA.size() != 2)
            throw Cnc_error("Can't edge flip a boundary");
        uint i = 0;
        for (const auto& f : FA){
            NE[i] = topology::get_other(f,edge);
            i++;
        }
    }
    remove_edge(edge);
    if (E.find(NE) != E.end()){
        throw Cnc_error("flipped edge already exists");
    }
    E.insert(NE);
    for (auto v : edge){
        auto NF = topology::assemble_face(NE,v);
        F.insert(NF);
        induce_face_connectivity(NF);
    }
    induce_edge_connectivity(NE);
    checkEuler();
    return NE;
}

cnc::algo::topology::vertex cnc::algo::geometry::Mesh2::edgeSplit(const cnc::algo::topology::edge &edge)
{
    checkConnectivity();
    auto AF = EtoF[edge];
    auto m = add_vertex(G->midPoint(edge));
    if (isBoundaryEdge(edge))
        boundary_vertices.insert(m);
    else
        interior_vertices.insert(m);
    remove_edge(edge);
    std::vector<topology::edge> new_edges(2);
    for (uint i = 0;i<2;i++)
        new_edges[i] = topology::edge{m,edge[i]};
    for (const auto& f : AF){
        auto o = topology::get_other(f,edge);
        auto EI = topology::edge{o,m};
        new_edges.push_back(EI);
        for (uint i = 0;i<2;i++){
            auto NF = topology::assemble_face(EI,edge[i]);
            F.insert(NF);
            induce_face_connectivity(NF);
        }
    }
    for (const auto& e : new_edges){
        E.insert(e);
        induce_edge_connectivity(e);
    }
    checkEuler();
    return m;
}

void cnc::algo::geometry::Mesh2::edgeCollapseInto(const cnc::algo::topology::edge &edge, cnc::algo::topology::vertex m)
{
    checkConnectivity();
    auto o = topology::get_other(edge,m);
    auto OR = getOneRingEdges(o);

    topology::edges new_edges;
    auto ORF = getOneRingFaces(o);
    remove_vertex(o);
    if (ORF.size() < 2)
        throw Cnc_error("suicide toi");
    else if (ORF.size() == 2)
        for (const auto& f : ORF){
            auto oe = topology::get_opposite_edge(f,o);
            induce_edge_connectivity(oe);
        }
    else {
        for (const auto& f : ORF){
            if (!belong(m,f)){
                auto NF = topology::assemble_face(topology::get_opposite_edge(f,o),m);
                for (const auto& e : NF)
                    new_edges.insert(e);
                insertFace(NF);
                induce_face_connectivity(NF);
            }
        }
        for (const auto& e : new_edges)
            induce_edge_connectivity(e);
    }
    checkEuler();
}

bool cnc::algo::geometry::Mesh2::shouldFlip(const cnc::algo::topology::edge &e) const
{
    if (EtoF.at(e).size() != 2)
        throw Cnc_error("face connectivy error");
    std::map<vertex,int> goal;
    for (uint i = 0;i<2;i++)
        goal[e[i]] = isBoundaryVertex(e[i]) ? 4 : 6;
    for (const auto& f : EtoF.at(e)){
        auto v = topology::get_other(f,e);
        goal[v] = isBoundaryVertex(v) ? 4 : 6;
    }
    int score = 0;
    for (uint i = 0;i<2;i++)
        score += std::abs(vertexValence(e[i]) - goal[e[i]]);
    for (const auto& f : EtoF.at(e)){
        auto v = topology::get_other(f,e);
        score += std::abs(vertexValence(v) - goal[v]);
    }
    int postscore = 0;
    for (uint i = 0;i<2;i++)
        postscore += std::abs(vertexValence(e[i])-1 - goal[e[i]]);
    for (const auto& f : EtoF.at(e)){
        auto v = topology::get_other(f,e);
        postscore += std::abs(vertexValence(v)+1 - goal[v]);
    }
    return postscore > score;
}
void cnc::algo::geometry::Mesh2::edgeCollapse(const cnc::algo::topology::edge &edge)
{
    auto OR = getOneRingEdges(edge[0]);
    auto OR2 = getOneRingEdges(edge[1]);
    OR.insert(OR2.begin(),OR2.end());

    auto m = add_vertex(G->midPoint(edge));
    topology::edges new_edges;
    for (uint i = 0;i<2;i++){
        const auto& ORF = getOneRingFaces(edge[i]);
        for (const auto& f : ORF){
            if (!belong(edge[!i],f)){
                auto NF = topology::assemble_face(topology::get_opposite_edge(f,edge[i]),m);
                for (const auto& e : NF)
                    new_edges.insert(e);
                insertFace(NF);
                induce_face_connectivity(NF);
            }
        }
    }
    for (const auto& e : OR)
        remove_edge(e);
    for (auto v : edge)
        remove_vertex(v);
    for (const auto& e : new_edges)
        induce_edge_connectivity(e);
}


int cnc::algo::geometry::Mesh2::vertexValence(cnc::algo::topology::vertex v) const
{
    checkConnectivity();
    return VtoE.at(v).size();
}

int cnc::algo::geometry::Mesh2::valenceScore(const cnc::algo::topology::edge &e) const
{
    checkConnectivity();
    int score = 0;
    for (uint i = 0;i<2;i++)
        score += std::abs(vertexValence(e[i]) - (isBoundaryVertex(e[i]) ? 4 : 6));
    for (const auto& f : EtoF.at(e)){
        auto v = topology::get_other(f,e);
        score += std::abs(vertexValence(v) - (isBoundaryVertex(v) ? 4 : 6));
    }
    return score;
}

cnc::vec cnc::algo::geometry::Mesh2::oneRingBarycenter(cnc::algo::topology::vertex v) const
{
    vec b(2);
    auto O = getOneRingVertices(v);
    for (auto v : O)
        b += (*G)(v);
    return b * 1./O.size();
}

cnc::scalar cnc::algo::geometry::Mesh2::maxInscribedRadius() const
{
    const auto& C = *getContext();
    return C.inscribedRadius(*std::max_element(F.begin(),F.end(),[C] (const face& a,const face& b){
        return C.inscribedRadius(a) < C.inscribedRadius(b);
    }));
}


void cnc::algo::geometry::Mesh2::induce_from_faces()
{
    E.clear();
    V.clear();
    for (const auto& f : F){
        for (const auto& e : f){
            E.insert(e);
            for (const auto& v : e)
                V.insert(v);
        }
    }
}

void cnc::algo::geometry::Mesh2::induce_face_connectivity(const cnc::algo::topology::face &f)
{
    for (const auto& e : f){
        auto tmp = EtoF[e];
        auto& c = EtoF[e];
        c.insert(f);
        if (c.size() > 2){
            throw Cnc_error("too much faces");
        }
        for (const auto& v : e){
            VtoF[v].insert(f);
            VtoE[v].insert(e);
        }
    }
}

void cnc::algo::geometry::Mesh2::induce_edge_connectivity(const cnc::algo::topology::edge &e)
{
    const auto& c = EtoF.at(e);
    auto n = c.size();
    if (n == 1)
        boundary.insert(e);
    else if (n == 2)
        interior_edges.insert(e);
    else
        throw Cnc_error("edge alone");
}

void cnc::algo::geometry::Mesh2::clear_connectivity()
{
    EtoF.clear();
    VtoF.clear();
    VtoE.clear();
    boundary.clear();
    interior_edges.clear();
    interior_vertices.clear();
    boundary_vertices.clear();
}

const cnc::algo::topology::vertices& cnc::algo::geometry::Mesh2::getInteriorVertices() const
{
    return interior_vertices;
}

cnc::cloud cnc::algo::geometry::Mesh2::getVertexCloud() const
{
    cloud X;
    const auto& C = *G;
    for (auto v : V)
        X.add_point(C(v));
    return X;
}

void cnc::algo::geometry::Mesh2::filterDeadVertices()
{
    clear_connectivity();
    std::map<vertex,vertex> NID;
    int i = 0;
    for (auto v : V){
        NID[v] = i;
        i++;
    }
    std::vector<vec> np(nbVertices());
    for (auto v : V)
        np[NID[v]] = (*G)(v);
    G->getPoints() = std::move(np);
    topology::faces NF;
    for (const auto& f : F){
        auto nf = f;
        for (uint i = 0;i<3;i++)
            for (uint j = 0;j<2;j++)
                nf[i][j] = NID[f[i][j]];
        NF.insert(nf);
    }
    F.clear();
    F = std::move(NF);
    induce_from_faces();
    compute_connectivity = false;
    computeConnectivity();
}

cnc::algo::topology::vertices cnc::algo::geometry::Mesh2::getBoundaryVertices() const
{
    return boundary_vertices;
}

void cnc::algo::geometry::Mesh2::remove_edge(const cnc::algo::topology::edge &edge)
{
    auto FA = EtoF[edge];
    for (const auto f : FA )
        remove_face(f);
    EtoF.erase(edge);
    for (auto v : edge)
        VtoE[v].erase(edge);
    E.erase(edge);
    boundary.erase(edge);
    interior_edges.erase(edge);
}

void cnc::algo::geometry::Mesh2::insert_edge(const cnc::algo::topology::edge &e)
{
    E.insert(e);
    if (compute_connectivity){
        if (EtoF.at(e).size() == 2)
            interior_edges.insert(e);
        else
            boundary.insert(e);
    }
}

void cnc::algo::geometry::Mesh2::remove_face(const cnc::algo::topology::face &f)
{
    for (const auto& e : f){
        EtoF[e].erase(f);
        for (auto v : e){
            VtoF[v].erase(f);
        }
    }
    F.erase(f);
}

cnc::algo::topology::vertex cnc::algo::geometry::Mesh2::add_vertex(const cnc::vec &x)
{
    auto v = G->add_vertex(x);
    V.insert(v);
    return v;
}

void cnc::algo::geometry::Mesh2::remove_vertex(cnc::algo::topology::vertex v)
{
    auto OE = VtoE.at(v);
    for (const auto& e : OE)
        remove_edge(e);
    V.erase(v);
    VtoF.erase(v);
    VtoE.erase(v);
    interior_vertices.erase(v);
    boundary_vertices.erase(v);
}
