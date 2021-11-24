#include "gridgraph.h"


GridGraph::GridGraph(const std::string &filename)
{
    std::ifstream f(filename);
    f >> W >> H;
    printf("%i %i\n",W,H);

    altitude.resize(W*H);
    djikstra_table.resize(W*H);
    for (auto& a : altitude)
        f >> a;
}

void GridGraph::perform_djikstra(int source)
{
    auto cmp = [](const djikstra_data& a,const djikstra_data& b) {
        return a.path_length < b.path_length;
    };
    //define priority queue
    std::priority_queue<djikstra_data, std::vector<djikstra_data>, decltype(cmp)> Q(cmp);

    //set ids in data table
    for (uint k = 0;k<djikstra_table.size();k++)
        djikstra_table[k].id = k;

    //set source
    djikstra_table[source].s = to_visit;
    djikstra_table[source].path_length = 0;

    //add source to queue
    Q.push(djikstra_table[source]);
    while (Q.size()){
        djikstra_data y = Q.top(); Q.pop();
        if (y.s == visited)
            continue;
        auto N = get_neighbors(y.id);
        for (int n_id : N){
            djikstra_data n = djikstra_table[n_id];
            if (n.s == visited)
                continue;
            float d = distance(y.id,n_id);
            float nd = y.path_length + d;
            if (n.s == unknown || n.path_length > nd){
                n.path_length = nd;
                n.s = to_visit;
                n.predecessor_id = y.id;
                Q.push(n);
            }
        }
        y.s = visited;
        djikstra_table[y.id] = y;
    }
}

void GridGraph::plot(cnc::euclid::EuclideanPlane *E)
{
    float x,y;
    for (int j = 0;j<H;j++){
        y = float(j)/H;
        for (int i = 0;i<W;i++){
            x = float(i)/W;
            auto P = E->add_object<cnc::euclid::Point>([x,y](){
                return cnc::vec({x,y});
            },2);
            float d = std::min(djikstra_table[get_index(i,j)].path_length/300.,1.0);
            P->set_color(QColor::fromRgb(d*255,0,(1-d)*255));
        }
    }
}

int GridGraph::get_index(int i, int j) const
{
    return j*W + i;
}

std::vector<int> GridGraph::get_neighbors(int i, int j)
{
    std::vector<int> N;
    if (i > 0)
        N.push_back(get_index(i-1,j));
    if (i < W)
        N.push_back(get_index(i+1,j));
    if (j > 0)
        N.push_back(get_index(i,j-1));
    if (j < H)
        N.push_back(get_index(i,j+1));
    return N;
}

std::vector<int> GridGraph::get_neighbors(int index)
{
    return get_neighbors(index%W,index/W);
}

float GridGraph::distance(int id1, int id2) const
{
    float hdiff = altitude[id1] -altitude[id2];
    return std::sqrt(1+hdiff*hdiff);
}

