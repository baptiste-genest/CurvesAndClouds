#ifndef D2_CURVE_H
#define D2_CURVE_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "curves.h"

namespace cnc {

/**
 * @brief The D2_curve class to plot curves in 2D
 */
class D2Curve : public Curves
{
public:
    /**
     * @brief D2_curve
     * @param X
     * @param Y
     */
    D2Curve(const cnc::algo::calculus::nodes& X,const cnc::algo::calculus::nodes& Y,bool color = false);
    ~D2Curve() {}


    inline  plot_type get_type() const override {
        return d2_curve;
    }

    inline void set_line_width(uint w) {
        Curves::line_width = w;
    }

protected:
    D2Curve() {}
    virtual void compute_value_range(const frame_info& fi) override;
    virtual void compute_values(const frame_info&) override {};

    void set_segment_color(frame_draw_object&,uint) const override;

    bool color_by_order;
};

}

#endif // D2_CURVE_H
