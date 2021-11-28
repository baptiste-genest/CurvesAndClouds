#ifndef GRAPHPLOTTER_H
#define GRAPHPLOTTER_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plottable.h"
#include "../../core/algo/graph/graph.h"
#include "../../core/algo/statistics/random_var.h"

#include <set>

namespace cnc {

class GraphPlotter : public Plottable
{
public:
    GraphPlotter(algo::Graph& g);

private:
    uint V;
    algo::Graph& ref;

    void plot(frame_draw_object&) override;

    void compute_nodes_pos();

    std::vector<std::complex<float>> nodes_pos;

    void compute_value_range(const frame_info&) override {}
    void compute_values(const frame_info&) override {}

    void call_draw_axis(frame_draw_object&) const override {}

    inline  plot_type get_type() const override {
        return graphplotter;
    }

    const scalar R = 10;

};

}

#endif // GRAPHPLOTTER_H
