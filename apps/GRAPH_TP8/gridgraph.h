#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H

#include <vector>
#include <string>
#include <fstream>

class GridGraph
{
public:
    GridGraph(const std::string& filename);

private:
    std::vector<int> altitude;
    int W,H;
};

#endif // GRIDGRAPH_H
