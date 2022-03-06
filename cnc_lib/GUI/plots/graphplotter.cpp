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
        nodes_pos[k] = std::polar(R,(scalar)2*k*M_PI/(V));
    using state = std::vector<pos>;
    state vel(V);
    float dt = 0.01f;
    static constexpr uint nb_iter = 200;

    uint V = ref.nb_nodes;
    static std::function<float(float)> S = [](float d){
        return 1.f/(1+exp(-5*(d-1)));
    };

    for (uint k = 0;k<nb_iter;k++){
        std::vector<pos> acc(V,pos());
        for (uint i = 0;i<V;i++){
            for (uint j = 0;j<V;j++){
                if (i != j){
                    pos dir = nodes_pos[j]-nodes_pos[i];
                    float d = std::abs(dir);
                    dir /= d;
                    if (ref.areConnected(i,j))
                        acc[i] += d*dir;
                    acc[i] += -dir*float(1.f/(d*d));
                }
            }
        }
        for (uint i = 0;i<V;i++){
            vel[i] += acc[i]*dt;
            nodes_pos[i] += vel[i]*dt;
        }
    }
}

