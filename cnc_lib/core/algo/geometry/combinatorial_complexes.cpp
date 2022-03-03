#include "combinatorial_complexes.h"

bool cnc::algo::topology::operator==(const cnc::algo::topology::edge &a, const cnc::algo::topology::edge &b){
    return ((a[0]==b[0]) && (a[1]==b[1]))||((a[1]==b[0]) && (a[0]==b[1]));
}

bool cnc::algo::topology::operator==(const cnc::algo::topology::face &A, const cnc::algo::topology::face &B){
    return get_vertices(A)==get_vertices(B);
}

cnc::algo::topology::vertices cnc::algo::topology::get_vertices(const cnc::algo::topology::face &F){
    vertices vertices;
    for (const auto& e : F)
        for (const auto& v : e)
            vertices.insert(v);
    return vertices;
}

cnc::algo::topology::vertex cnc::algo::topology::get_other(const cnc::algo::topology::face &F, const cnc::algo::topology::edge &e) {
    for (const auto& v : get_vertices(F))
        if (e[0] != v && e[1] != v)
            return v;
    throw "no other vertex";
}

cnc::algo::topology::edge cnc::algo::topology::get_common_edge(const cnc::algo::topology::face &F, cnc::algo::topology::vertex O){
    for (const auto& e : F)
        if (e[0] != O && e[1] != O)
            return e;
    throw "no common edge";
}

cnc::algo::topology::indexed_vertices cnc::algo::topology::get_indexed_vertices(const cnc::algo::topology::face &F)
{
    auto V = get_vertices(F);
    return indexed_vertices(V.begin(),V.end());
}

cnc::algo::topology::face cnc::algo::topology::assemble_face(const cnc::algo::topology::edge &E, const cnc::algo::topology::vertex &x)
{
    if (E[0] == E[1] || E[0] == x || E[1] == x)
        throw Cnc_error("Can't build face without 3 distinct vertices");
    return {E,{E[0],x},{E[1],x}};
}

bool cnc::algo::topology::faceComp::operator()(const cnc::algo::topology::face &a, const cnc::algo::topology::face &b)
{
    auto va = get_indexed_vertices(a);
    auto vb = get_indexed_vertices(b);
    for (int i = 0;i<3;i++){
        if (va[i] != vb[i])
            return va[i] < vb[i];
    }
    return false;
}
bool cnc::algo::topology::faceComp::operator()(const cnc::algo::topology::face &a, const cnc::algo::topology::face &b) const
{
    auto va = get_indexed_vertices(a);
    auto vb = get_indexed_vertices(b);
    for (int i = 0;i<3;i++){
        if (va[i] != vb[i])
            return va[i] < vb[i];
    }
    return false;
}


bool cnc::algo::topology::edgeComp::operator()(const cnc::algo::topology::edge &a, const cnc::algo::topology::edge &b)
{
    indexed_vertices va(a.begin(),a.end());
    indexed_vertices vb(b.begin(),b.end());
    std::sort(va.begin(),va.end());
    std::sort(vb.begin(),vb.end());
    for (int i = 0;i<2;i++){
        if (va[i] != vb[i])
            return va[i] < vb[i];
    }
    return false;
}
bool cnc::algo::topology::edgeComp::operator()(const cnc::algo::topology::edge &a, const cnc::algo::topology::edge &b) const
{
    indexed_vertices va(a.begin(),a.end());
    indexed_vertices vb(b.begin(),b.end());
    std::sort(va.begin(),va.end());
    std::sort(vb.begin(),vb.end());
    for (int i = 0;i<2;i++){
        if (va[i] != vb[i])
            return va[i] < vb[i];
    }
    return false;
}


bool cnc::algo::topology::belong(cnc::algo::topology::vertex x, const cnc::algo::topology::face &F)
{
    for (auto v : get_vertices(F))
        if (x == v)
            return true;
    return false;
}

bool cnc::algo::topology::belong(cnc::algo::topology::vertex x, const cnc::algo::topology::edge &E)
{
    return x == E[0] || x == E[1];
}

cnc::algo::topology::vertex cnc::algo::topology::get_other(const cnc::algo::topology::edge &E, cnc::algo::topology::vertex x)
{
    if (x == E[0])
        return E[1];
    if (x != E[1])
        throw Cnc_error("vertex doesn't belong to edge");
    return E[0];
}

cnc::algo::topology::edge cnc::algo::topology::completeCycle(const cnc::algo::topology::SimplicialPolygon &P)
{
    if (P.empty())
        return NULL_EDGE;
    std::map<vertex,int> counter;
    for (const auto& e : P){
        for (int i = 0;i<2;i++)
            if (counter.find(e[i]) == counter.end())
                counter[e[i]] = 1;
            else
                counter[e[i]]++;
    }
    std::vector<int> alones;
    for (const auto& c : counter){
        if (c.second == 1)
            alones.push_back(c.first);
    }
    if (alones.size() > 2)
        throw Cnc_error("More than 1 edge missing in polygon");
    if (alones.empty())
        return NULL_EDGE;
    return {alones[0],alones[1]};
}

void cnc::algo::topology::swap(cnc::algo::topology::edge &E)
{
    auto tmp = E[0];
    E[0] = E[1];
    E[1] = tmp;
}

cnc::algo::topology::indexed_vertices cnc::algo::topology::get_indexed_vertices(const cnc::algo::topology::SimplicialPolygon &P)
{
    vertices V = get_vertices(P);
    return indexed_vertices(V.begin(),V.end());
}

cnc::algo::topology::vertices cnc::algo::topology::get_vertices(const cnc::algo::topology::SimplicialPolygon &P)
{
    vertices V;
    for (const auto& E : P){
        V.insert(E[0]);
        V.insert(E[1]);
    }
    return V;
}

cnc::algo::topology::face cnc::algo::topology::assemble_face(cnc::algo::topology::vertex x1, cnc::algo::topology::vertex x2, cnc::algo::topology::vertex x3)
{
    if (x1 == x2 || x1 == x3 || x2 == x3)
        throw Cnc_error("Can't build face without 3 distinct vertices");
    face F;
    F[0] = {x1,x2};
    F[1] = {x1,x3};
    F[2] = {x2,x3};
    return F;
}

bool cnc::algo::topology::belong(const cnc::algo::topology::edge &E, const cnc::algo::topology::face &F)
{
    for (const auto& e : F)
        if (e==E)
            return true;
    return false;
}

std::array<cnc::algo::topology::edge,2> cnc::algo::topology::get_other_edges(const cnc::algo::topology::face &f, const cnc::algo::topology::edge &e)
{
    std::array<cnc::algo::topology::edge,2> others;
    uint i = 0;
    for (const auto& ed : f)
        if (!(ed == e)){
            others[i] = ed;
            i++;
        }
    return others;
}

cnc::algo::topology::edge cnc::algo::topology::get_opposite_edge(const cnc::algo::topology::face &F, cnc::algo::topology::vertex O)
{
    for (const auto& e : F)
        if (!belong(O,e))
            return e;
    return NULL_EDGE;
}

void cnc::algo::topology::remove_vertex(faces &F, vertex x)
{
    std::vector<face> to_delete;
    for (const auto& f : F)
        if (belong(x,f))
            to_delete.push_back(f);
    for (const auto& f : to_delete)
        F.erase(f);
}
