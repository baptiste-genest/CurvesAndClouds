#include "combinatorial_complexes.h"

bool cnc::algo::topo::operator==(const cnc::algo::topo::edge &a, const cnc::algo::topo::edge &b){
    return ((a[0]==b[0]) && (a[1]==b[1]))||((a[1]==b[0]) && (a[0]==b[1]));
}

bool cnc::algo::topo::operator==(const cnc::algo::topo::face &A, const cnc::algo::topo::face &B){
    return get_vertices(A)==get_vertices(B);
}

cnc::algo::topo::vertices cnc::algo::topo::get_vertices(const cnc::algo::topo::face &F){
    vertices vertices;
    for (const auto& e : F)
        for (const auto& v : e)
            vertices.insert(v);
    return vertices;
}

cnc::algo::topo::vertex cnc::algo::topo::get_other(const cnc::algo::topo::face &F, const cnc::algo::topo::edge &e) {
    for (const auto& v : get_vertices(F))
        if (e[0] != v && e[1] != v)
            return v;
    throw "no other vertex";
}

cnc::algo::topo::edge cnc::algo::topo::get_common_edge(const cnc::algo::topo::face &F, cnc::algo::topo::vertex O){
    for (const auto& e : F)
        if (e[0] != O && e[1] != O)
            return e;
    throw "no common edge";
}
