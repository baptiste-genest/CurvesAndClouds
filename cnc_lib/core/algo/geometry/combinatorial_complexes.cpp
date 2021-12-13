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
