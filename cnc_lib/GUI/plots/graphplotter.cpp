#include "graphplotter.h"

cnc::GraphPlotter::GraphPlotter(cnc::algo::Graph &g) : ref(g)
{
    V = g.nb_nodes;
    nodes_pos.resize(V);
    compute_nodes_pos();
}

void cnc::GraphPlotter::plot(cnc::frame_draw_object &fdo)
{
    static auto toP = [] (const std::complex<float>& x){
        return QPointF(x.real(),x.imag());
    };

    auto M = algo_GUI::gen_mapping_value_to_frame({-R,R},{-R,R},fdo.fi);
    for (uint i = 0;i<V;i++){
        auto p = nodes_pos[i];
        for (auto n : ref.successors[i])
            fdo.painter.drawLine(M(toP(p)),M(toP(nodes_pos[n.first])));
    }
    for (const auto& p : nodes_pos)
        fdo.painter.drawEllipse(M(toP(p)),5,5);
}

void cnc::GraphPlotter::compute_nodes_pos()
{
    using pos = std::complex<float>;
    using namespace algo::stat::random_var;
    for (uint k = 0;k<V;k++)
        nodes_pos[k] = std::polar(R,2*k*M_PI/(V));
    static constexpr uint nb_iter = 10;

    uint V = ref.nb_nodes;

    for (uint k = 0;k<nb_iter;k++){
        std::vector<pos> npos(V,pos());
        for (uint i = 0;i<V;i++){
            for (auto n : ref.edges[i])
                npos[i] += nodes_pos[n];
            npos[i] /= ref.edges[i].size();

            for (uint j = 0;j<V;j++)
                if (i != j){
                    auto D = (npos[i] - nodes_pos[j]);
                    auto d = std::abs(D);
                    npos[i] += 0.1f*D/(d*d);
                }
        }
        nodes_pos = npos;
    }
}

