#include "mesh2.h"

cnc::algo::geometry::Mesh2::Mesh2(GeometricContextRef C, const topology::faces &Fa, const EdgeFaceConnectivityGraph &EF, const NormalMap &NM) : G(C),F(Fa),EtoF(EF),N(NM)
{
    for (const auto& f : F)
        for (const auto& e : f){
            E.insert(e);
            for (const auto& v : e)
                V.insert(v);
        }
}

cnc::algo::geometry::Mesh2::Mesh2(cnc::algo::geometry::Mesh2 &&O) : G(O.G)
{
    std::cout << "mesh moved" << std::endl;
    V = std::move(O.V);
    E = std::move(O.E);
    F = std::move(O.F);
    boundary = std::move(O.boundary);
    interior_edges = std::move(O.interior_edges);
    EtoF = std::move(O.EtoF);
    N = std::move(O.N);
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
            newN[f] = N.at(f);
        }
    N = std::move(newN);
    V.clear();
    E.clear();
    boundary.clear();
    interior_edges.clear();
    EtoF.clear();
    induce_from_faces();
}

const cnc::algo::topology::faces& cnc::algo::geometry::Mesh2::faces() const
{
    return F;
}

const cnc::algo::topology::faces &cnc::algo::geometry::Mesh2::getFaces(const topology::edge &e) const
{
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

void cnc::algo::geometry::Mesh2::insertFace(const cnc::algo::topology::face &f)
{
    F.insert(f);
    for (const auto& e : f){
        E.insert(e);
        for (auto v : e)
            V.insert(v);
    }
}

void cnc::algo::geometry::Mesh2::induce_from_faces()
{
    for (const auto& f : F){
        for (const auto& e : f){
            E.insert(e);
            for (const auto& v : e)
                V.insert(v);
            EtoF[e].insert(f);
        }
    }
    for (const auto& ef : EtoF){
        if (ef.second.size() == 1)
            boundary.insert(ef.first);
        else
            interior_edges.insert(ef.first);
    }

}
