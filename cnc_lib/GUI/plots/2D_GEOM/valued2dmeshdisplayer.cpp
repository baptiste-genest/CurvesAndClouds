#include "valued2dmeshdisplayer.h"

cnc::Valued2DMeshDisplayer::Valued2DMeshDisplayer(cnc::algo::geometry::MeshRef m, const cnc::vec &v) : Mesh2DDisplayer(m)
{
    vertex_color.resize(v.size());
    up = [v] () {
        return v;
    };
    Valued2DMeshDisplayer::compute_values({});
}

cnc::Valued2DMeshDisplayer::Valued2DMeshDisplayer(cnc::algo::geometry::MeshRef m, const cnc::vec &v, cnc::range rv) : Mesh2DDisplayer(m)
{
    vertex_color.resize(v.size());
    up = [v] () {
        return v;
    };
    fixed_value_range = true;
    vrange = rv;
    Valued2DMeshDisplayer::compute_values({});
}

cnc::Valued2DMeshDisplayer::Valued2DMeshDisplayer(cnc::algo::geometry::MeshRef m, const valueUpdater &U, range rv) : Mesh2DDisplayer(m),up(U)
{
    Plottable::dynamic = true;
    vertex_color.resize(m->nbVertices());
    fixed_value_range = true;
    vrange = rv;
    Valued2DMeshDisplayer::compute_values({});
}

void cnc::Valued2DMeshDisplayer::plot(cnc::frame_draw_object &fdo)
{
    using namespace algo::topology;
    algo_GUI::mapping Mapper = algo_GUI::gen_mapping_value_to_frame(x_range,y_range, fdo.fi);
    const auto& G = *M->getContext();
    //fdo.painter.setPen(QPen(Plottable::plot_color,1));
    fdo.painter.setPen(Qt::NoPen);
    //const static auto nill = QColor::fromRgb(0,0,0,0);
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
        for (const auto& e : f){
            fdo.painter.setBrush(grad[e]);
            fdo.painter.drawConvexPolygon(p);
        }
    }
}

void cnc::Valued2DMeshDisplayer::compute_values(const cnc::frame_info &)
{
    auto values = up();
    if (!fixed_value_range){
        const auto& x = values.data();
        vrange.first = *std::min_element(x.begin(),x.end());
        vrange.second = *std::max_element(x.begin(),x.end());
    }

    auto R = algo::calculus::build_ordered_range_mapper({0,vrange.second},{0,255},true);
    cnc::algo::calculus::scalar_function_1D B;
    if (vrange.first < 0)
        B = algo::calculus::build_range_mapper({vrange.first,0},{255,0},true);
    else
        B = [] (scalar) {return 0.;};
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

cnc::VectorValued2DMeshDisplayer::VectorValued2DMeshDisplayer(cnc::algo::geometry::MeshRef m, const cnc::algo::FEM::VectorSample &Values) : Mesh2DDisplayer(m),V(Values)
{

}

void cnc::VectorValued2DMeshDisplayer::plot(cnc::frame_draw_object &fdo)
{
    algo_GUI::mapping Mapper = algo_GUI::gen_mapping_value_to_frame(x_range,y_range, fdo.fi);
    fdo.painter.setPen(QPen(Plottable::plot_color,1));
    const auto& G = *M->getContext();
    for (const auto& e : M->edges()){
        auto P1 =Mapper(algo_GUI::vec_to_QPointF(G(e[0]) + V.at(e[0])));
        auto P2 =Mapper(algo_GUI::vec_to_QPointF(G(e[1])+ V.at(e[1])));
        fdo.painter.drawLine(P1,P2);
    }
}

void cnc::VectorValued2DMeshDisplayer::compute_values(const cnc::frame_info &)
{

}
