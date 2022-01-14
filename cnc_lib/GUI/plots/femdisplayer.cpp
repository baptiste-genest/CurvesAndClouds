#include "femdisplayer.h"

cnc::FEMDisplayer::FEMDisplayer(const algo::FEM::ScalarFiniteElementSolver &F) : solver(F)
{
    const static auto conv = [] (const vec& x){
        return QPointF(x(0),x(1));
    };
    const auto& M = F.getMesh();
    auto D = algo::geometry::Voronoi(M.getContext()->getPoints(),0);

    centroids_color.resize(M.nbVertices(),QColorConstants::Black);

    auto z_range = F.SolutionValueRange();
    auto R = algo::calculus::build_range_mapper({0,z_range.second},{0,255},true);
    auto B = algo::calculus::build_range_mapper({z_range.first,0},{255,0},true);

    uint i = 0;
    for (auto v : M.interiorVertices()){
        scalar z = F.solutionValueAtVertex(i);
        centroids_color[i] = QColor::fromRgb(R(z),0,B(z));
        QVector<QPointF> points;
        for (const auto& x : D.getCell(v).getIndexedPoints())
            points.push_back(conv(x));
        cells.push_back(points);
        i++;
    }
}

void cnc::FEMDisplayer::plot(frame_draw_object &fdo)
{
    algo_GUI::mapping Mapper = algo_GUI::gen_mapping_value_to_frame(x_range,y_range, fdo.fi);
    fdo.painter.setPen(QPen(QColorConstants::Black,0));
    for (uint i = 0;i<cells.size();i++){
        fdo.painter.setBrush(centroids_color[i]);
        auto pcell = cells[i];
        for (auto& x : pcell)
            x = Mapper(x);
        fdo.painter.drawConvexPolygon(pcell);
    }
}

void cnc::FEMDisplayer::compute_value_range(const frame_info &){
    const auto& M = solver.getMesh();
    uint N = M.nbVertices();
    const auto& C = *M.getContext();
    std::vector<scalar> coord(N);
    for (int i = 0;i<(int)N;i++)
        coord[i] = C(i)(0);
    x_range = algo::get_min_max_range(coord);
    for (int i = 0;i<(int)N;i++)
        coord[i] = C(i)(1);
    y_range = algo::get_min_max_range(coord);
}
