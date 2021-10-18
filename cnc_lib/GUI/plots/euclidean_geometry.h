#ifndef EUCLIDEAN_GEOMETRY_H
#define EUCLIDEAN_GEOMETRY_H

#include <QPainter>
#include "cnc_types.h"
#include "cnc_error.h"
#include "plottable.h"
#include "core/algo/lin_alg.h"

namespace cnc {

namespace euclid {

using placer = std::function<vec()>;

class EuclieanPrimitive : public Plottable {
public:
    inline plot_type get_type() const override{ return plot_type::d2_curve;}
private:
    void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const override{}
    virtual void call_draw_axis(frame_draw_object&) const override{}

    virtual void plot(frame_draw_object& fdo) override = 0;
    virtual void compute_values(const frame_info&) override = 0;
    virtual void compute_value_range(const frame_info& fi) override = 0;

};

class Point : public EuclieanPrimitive {
public:
    Point(const placer& p,int rad) : pos_updater(p),radius(rad){}
private:
    vec pos;
    placer pos_updater;
    int radius;

    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override;
    virtual void compute_value_range(const frame_info& fi) override;
};

class Line : public EuclieanPrimitive {
public:
    Line(const placer& p1,const placer& p2,int w) :
        pos1_updater(p1),
        pos2_updater(p2),
        width(w){
        dynamic = true;
        pos1 = vec(2);
        pos2 = vec(2);
    }
private:
    vec pos1,pos2;
    placer pos1_updater;
    placer pos2_updater;
    int width;

    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override;
    virtual void compute_value_range(const frame_info& fi) override;
};

}

}
#endif // EUCLIDEAN_GEOMETRY_H
