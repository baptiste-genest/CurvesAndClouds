#include "graph.h"

using namespace cnc::algo::graph;

cnc::algo::Graph::Graph(int V,bool W) : nb_nodes(V), weighted(W)
{
    predecessors.resize(V);
    successors.resize(V);
    edges.resize(V);
}

void cnc::algo::Graph::addEdge(cnc::algo::graph::nodeid a, cnc::algo::graph::nodeid b,scalar)
{
    addArc(a,b);
    addArc(b,a);
}

void cnc::algo::Graph::addArc(cnc::algo::graph::nodeid a, cnc::algo::graph::nodeid b,scalar w)
{
    setConnectivity(a,b);
    predecessors[b][a] = w;
    successors[a][b] = w;
}

cnc::algo::graph::arcsref cnc::algo::Graph::getNeighbours(cnc::algo::graph::nodeid n) const
{
    return successors[n];
}

bool cnc::algo::Graph::areConnected(nodeid a, nodeid b) const
{
    return edges[a].find(b) != edges[a].end();
}

path cnc::algo::Graph::DFS(nodeid s) const
{
    std::vector<visit_status> ND(nb_nodes,unknown);
    path P;
    std::stack<nodeid> S;
    S.push(s);
    while (!S.empty()){
        nodeid c = S.top();S.pop();
        ND[c] = visited;
        P.push_back(c);
        for (const auto& a : successors[c]){
            if (ND[a.first] == unknown){
                S.push(a.first);
                ND[a.first] = to_visit;
            }
        }
    }
    return P;
}

void cnc::algo::Graph::setConnectivity(nodeid a, nodeid b)
{
    edges[a].insert(b);
    edges[b].insert(a);
}
