#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H

#include "curvesandcloud.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <math.h>

enum visit_status {
    unknown,
    visited,
    to_visit
};

struct djikstra_data{
    int id;
    visit_status s = unknown;
    int predecessor_id = -1;
    float path_length = -1;
};

class GridGraph
{
public:
    GridGraph(const std::string& filename);

    void perform_djikstra(int source);

    void plot(cnc::euclid::EuclideanPlane* E,int goal);
    inline int get_index(int i,int j) const;

private:

    std::vector<int> get_neighbors(int i,int j);
    std::vector<int> get_neighbors(int index);
    float distance(int id1,int id2) const;

    std::vector<float> altitude;
    std::vector<djikstra_data> djikstra_table;
    int W,H;
};

#endif // GRIDGRAPH_H
