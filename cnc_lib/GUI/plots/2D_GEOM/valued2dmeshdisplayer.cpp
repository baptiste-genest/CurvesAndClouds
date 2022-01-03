#include "valued2dmeshdisplayer.h"

cnc::Valued2DMeshDisplayer::Valued2DMeshDisplayer(cnc::meshref m, const cnc::vec &v) : Mesh2DDisplayer(m),values(v)
{
    vertex_color.resize(v.size());
    Valued2DMeshDisplayer::compute_values({});
}

void cnc::Valued2DMeshDisplayer::plot(cnc::frame_draw_object &fdo)
{
    using namespace algo::topology;
    algo_GUI::mapping Mapper = algo_GUI::gen_mapping_value_to_frame(x_range,y_range, fdo.fi);
    const auto& G = *M->getContext();
    fdo.painter.setPen(QPen(Plottable::plot_color,1));
    const static auto nill = QColor::fromRgb(0,0,0,0);
    for (const auto& f : M->F){
        QVector<QPointF> p;
        std::map<vertex,QPoint> P;
        for (auto v : get_vertices(f)){
            P[v]= Mapper(algo_GUI::vec_to_QPointF(G(v)));
            p.append(P[v]);
        }
        std::map<edge,QLinearGradient,edgeComp> grad;
        for (const auto& e : f){
            grad[e] = QLinearGradient(P[e[0]],P[e[1]]);
            grad[e].setColorAt(0,vertex_color[e[0]]);
            grad[e].setColorAt(1,vertex_color[e[1]]);
        }
        /*
        for (auto v : get_vertices(f)){
            auto pv = algo_GUI::vec_to_QPointF(G(v));
            grad[v] = QLinearGradient(Mapper(pv),Mapper(algo_GUI::vec_to_QPointF(G.projectOnEdge(v,get_opposite_edge(f,v)))));
            grad[v].setColorAt(0,vertex_color[v]);
            //grad[v].setColorAt(0.8,face_color[f]);
            grad[v].setColorAt(1,nill);
            p.append(Mapper(pv));
        }
        */
        for (const auto& e : f){
            fdo.painter.setBrush(grad[e]);
            fdo.painter.drawConvexPolygon(p);
        }
    }
}

void cnc::Valued2DMeshDisplayer::compute_values(const cnc::frame_info &)
{
    const auto& x = values.data();
    vrange.first = *std::min_element(x.begin(),x.end());
    vrange.second = *std::max_element(x.begin(),x.end());

    auto R = algo::calculus::build_range_mapper({0,vrange.second},{0,255},true);
    auto B = algo::calculus::build_range_mapper({vrange.first,0},{255,0},true);
    for (uint i = 0;i<values.size();i++){
        vertex_color[i] = QColor::fromRgb(R(values(i)),0,B(values(i)),255/2);
    }
    for (const auto& f : M.get()->F){
        scalar vf = 0;
        for (auto v : algo::topology::get_vertices(f))
            vf += values(v);
        vf *= 0.333333;
        face_color[f] = QColor::fromRgb(R(vf),0,B(vf),255);
    }
}
