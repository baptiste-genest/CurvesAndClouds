#ifndef GRAPH_H
#define GRAPH_H

#include "../../../cnc_error.h"
#include "../../../cnc_types.h"

#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <set>


namespace cnc{

class GraphPlotter;

namespace algo{

namespace graph {

using nodeid = int;

using arcs = std::map<nodeid,scalar>;
using arcsref = const std::map<nodeid,scalar>&;
using path = std::vector<nodeid>;
}

class Graph
{
public:
    Graph(int V,bool W = false);

    void addEdge(graph::nodeid a,graph::nodeid b,scalar w = 1);
    void addArc(graph::nodeid a,graph::nodeid b,scalar w = 1);

    graph::arcsref getNeighbours(graph::nodeid n) const;
    bool areConnected(graph::nodeid a,graph::nodeid b) const;

    graph::path DFS(graph::nodeid s) const;

private:

    void setConnectivity(graph::nodeid a,graph::nodeid b);

    enum visit_status{
        unknown,
        to_visit,
        visited
    };

    struct nodedata{
        visit_status s;
        graph::nodeid id;
    };

    void reset_exploration();

    int nb_nodes;
    bool weighted;
    std::vector<graph::arcs> predecessors;
    std::vector<graph::arcs> successors;
    std::vector<std::set<int>> edges;

    friend class cnc::GraphPlotter;
};

}

}

#endif // GRAPH_H
