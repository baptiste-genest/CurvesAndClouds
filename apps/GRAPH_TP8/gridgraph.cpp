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
    using djk = std::pair<int,float>;
    auto cmp = [](const djk& a,const djk& b) {
        return a.second > b.second;
    };
    //define priority queue
    std::priority_queue<djk, std::vector<djk>, decltype(cmp)> Q(cmp);

    //set ids in data table
    for (uint k = 0;k<djikstra_table.size();k++)
        djikstra_table[k].id = k;

    //set source
    djikstra_table[source].s = to_visit;
    djikstra_table[source].path_length = 0;

    //add source to queue
    Q.push({source,0.f});
    while (Q.size()){
        djk H = Q.top(); Q.pop();
        auto y = djikstra_table[H.first];
        if (djikstra_table[y.id].s == visited)
            continue;
        djikstra_table[y.id].s = visited;
        for (int n_id : get_neighbors(y.id)){
            djikstra_data& n = djikstra_table[n_id];
            if (n.s == visited)
                continue;
            float d = distance(y.id,n_id);
            float nd = y.path_length + d;
            if (n.s == unknown || n.path_length > nd){
                n.path_length = nd;
                n.s = to_visit;
                n.predecessor_id = y.id;
                Q.push({n_id,n.path_length});
            }
        }
    }
}

void GridGraph::plot(cnc::euclid::EuclideanPlane *E, int goal)
{
    float max_d = std::max_element(djikstra_table.begin(),djikstra_table.end(),[](const djikstra_data& A,const djikstra_data& B){
        return A.path_length < B.path_length;
    })->path_length;
    float x,y;
    std::vector<cnc::euclid::Point*> points(W*H);
    for (int j = 0;j<H;j++){
        y = float(j)/H;
        for (int i = 0;i<W;i++){
            x = float(i)/W;
            points[get_index(i,j)] = E->add_object<cnc::euclid::Point>([x,y](){
                return cnc::vec({x,y});
            },10);
            //float d = djikstra_table[get_index(i,j)].path_length/max_d;
            float d = std::min(1.f,djikstra_table[get_index(i,j)].path_length/30);
            points[get_index(i,j)]->set_color(QColor::fromRgb(d*255,0,(1-d)*255));
        }
    }
    int id = goal,next;
    while (true){
        next = djikstra_table[id].predecessor_id;
        if (next == -1)
            return;
        E->add_object<cnc::euclid::Segment>(points[next],points[id]);
        id = next;
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
    if (i < W-1)
        N.push_back(get_index(i+1,j));
    if (j > 0)
        N.push_back(get_index(i,j-1));
    if (j < H-1)
        N.push_back(get_index(i,j+1));
    return N;
}

std::vector<int> GridGraph::get_neighbors(int index)
{
    return get_neighbors(index%W,index/W);
}

float GridGraph::distance(int id1, int id2) const
{
    float hdiff = altitude[id1] - altitude[id2];
    return std::sqrt(hdiff*hdiff + 1);
}

