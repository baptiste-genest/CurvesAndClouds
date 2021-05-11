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
class D2_curve : public Curves
{
public:
    /**
     * @brief D2_curve
     * @param X
     * @param Y
     */
    D2_curve(const cnc::algo::calculus::nodes& X,const cnc::algo::calculus::nodes& Y);
    ~D2_curve() {}


    inline  plot_type get_type() const override {
        return d2_curve;
    }

private:
    void compute_value_range(const frame_info& fi) override;
    void compute_values(const frame_info&) override {};
};

}

#endif // D2_CURVE_H
