#include "mesh2.h"

cnc::algo::geometry::Mesh2::Mesh2(const GeometricContext &C, const topology::faces &Fa, const EdgeFaceConnectivityGraph &EF, const NormalMap &NM) : G(C),F(Fa),EtoF(EF),N(NM)
{
    for (const auto& f : F)
        for (const auto& e : f){
            E.insert(e);
            for (const auto& v : e)
                V.insert(v);
        }
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

const cnc::algo::topology::faces& cnc::algo::geometry::Mesh2::faces() const
{
    return F;
}

const cnc::algo::topology::faces &cnc::algo::geometry::Mesh2::getFaces(const topology::edge &e) const
{
    return EtoF.at(e);
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
