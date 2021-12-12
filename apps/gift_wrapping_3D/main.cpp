#include <curvesandcloud.h>

using namespace cnc;

using vertex = int;
using vertices = std::set<vertex>;
using edge = std::array<vertex,2>;
using face = std::array<edge,3>;

class GeometryContext {

static vertices get_vertices(const face& F){
    vertices vertices;
    for (const auto& e : F)
        for (const auto& v : e)
            vertices.insert(v);
    return vertices;
}

static edge get_common_edge(const face& F,vertex O){
    for (const auto& e : F)
        if (e[0] != O && e[1] != O)
            return e;
    throw "no common edge";
}

static inline vec get_vec_edge(const edge& e){
    return points[e[1]] - points[e[0]];
}

static vertex get_other(const face& F,const edge& e) {
    for (const auto& v : get_vertices(F))
        if (e[0] != v && e[1] != v)
            return v;
    throw "no other vertex";
}

static vec face_direction(const edge& E,vertex O){
    auto ev = get_vec_edge(E).normalize();
    auto ov = get_vec_edge({E[0],O}).normalize();
    return algo::cross(algo::cross(ev,ov),ev);
}

static scalar facet_angle(const face& F,vertex O){
    auto CE = get_common_edge(F,O);
    auto CEv = get_vec_edge(CE);
    return algo::dot(face_direction(CE,O),face_direction(CE,get_other(F,CE)));
}

private:
    static std::vector<vec> points;

};

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    tex_stream ts;
    ts << "My first plot!" << tex::endl;

    L->add_text_frame_from_tex_stream(ts);

    w.show();
    return App.exec();
}
