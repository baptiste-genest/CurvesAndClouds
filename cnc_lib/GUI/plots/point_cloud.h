/**
  * @file point_cloud.h class defining point cloud plots
  */
#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plottable.h"
#include "core/algo/statistics/statistics.h"
#include "GUI/algo/algo_gui.h"

namespace cnc {

class PointCloud : public Plottable
{
public:
    /**
     * @brief Points_cloud's constructor with a unique size of radius
     * @param coords
     * @param radius
     */
    PointCloud(const cloud& c, const uint radius);

    /**
     * @brief Points_cloud's constructor with a different size for each point
     * @param coords
     * @param radius
     */
    PointCloud(const cloud& coords, const std::vector<uint>& radius);

    inline  plot_type get_type() const override {
        return point_cloud;
    }

protected:

    void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const override;

private:
    constexpr static float zoom_factor = 1.2;

    virtual void call_draw_axis(frame_draw_object&) const override;

    void plot(frame_draw_object&) override;

    virtual void compute_values(const frame_info&) override {}
    virtual void compute_value_range(const frame_info& fi) override;

    std::vector<uint> point_radius;
    cloud coords;
    algo_GUI::Qcloud projected_2d_coords;
    uint size;
    bool uniform_radius;
};

}

#endif // POINT_CLOUD_H
