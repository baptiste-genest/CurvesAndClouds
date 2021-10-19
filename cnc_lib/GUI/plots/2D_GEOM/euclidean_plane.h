#ifndef EUCLIDEAN_PLANE_H
#define EUCLIDEAN_PLANE_H

#include "../plottable.h"
#include "euclidean_geometry.h"

namespace cnc {

namespace euclid {

class EuclideanPlane : public Plottable
{
public:
    inline plot_type get_type() const override{ return plot_type::d2_curve;}
    /**
     * @brief EuclideanPlane fixed range plane
     * @param x
     * @param y
     */
    EuclideanPlane(range x,range y);
    /**
     * @brief EuclideanPlane dynamically ranged plane
     */
    EuclideanPlane() : fixed_range(false){
        dynamic = true;
    }

    template <typename PrimitiveType,typename ... Args>
    PrimitiveType* add_object(Args&& ... args) {
        PrimitiveType* P = new PrimitiveType(std::forward<Args>(args)...);
        m_objects.push_back(P);
        return P;
    }

private:
    bool fixed_range = false;

    void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const override{}
    virtual void call_draw_axis(frame_draw_object&) const override{}

    virtual void plot(frame_draw_object& fdo) override;
    virtual void compute_values(const frame_info&) override;
    virtual void compute_value_range(const frame_info& fi) override;

    std::vector<EuclideanPrimitive*> m_objects;
};


}

}

#endif // EUCLIDEAN_PLANE_H
