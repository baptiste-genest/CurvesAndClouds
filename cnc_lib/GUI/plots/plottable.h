/**
  * @file plottable.h defines mother class for various plot types
  */
#ifndef PLOTTABLE_H
#define PLOTTABLE_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <string>
#include <QPainter>
#include <QColor>
#include <QWidget>
#include <regex>

#include "cnc_error.h"
#include "core/algo/calculus/calculus.h"
#include "cnc_types.h"
#include "GUI/display_info.h"
#include "GUI/algo/algo_gui.h"
#include "QVBoxLayout"

namespace cnc {

using namespace QColorConstants;

static constexpr uint NB_CNC_COLORS = 10;
static constexpr QColor CNC_COLORS[NB_CNC_COLORS] = {Blue,Red,Green,DarkBlue,DarkRed,DarkGreen,Magenta,Cyan,DarkMagenta,DarkCyan};

class PlotLayer;
class PlotFrame;
class StatList;
class PlotGroup;

enum plot_type {
    function_plot,
    d2_curve,
    scalar_field,
    vector_field,
    boolean_field,
    text,
    figure,
    point_cloud
};

/*! \class Plottable
 *  \brief Abstract class that represents any type plot.
 */
class Plottable : protected QWidget
{
public:
    Plottable();

    /*! \fn void change_label(axis axis_id, const std::string& str)
     *  \brief A function to change the name of a label.
     *  \param axis_id is either 'x_axis' or 'y_axis'.
     *  \param str is the new name of the axis.
     */
    virtual void change_label(axis axis_id, const std::string& str);

    virtual ~Plottable() {}

    /**
     * @brief set_color change color of the plot
     * @param qc qt color
     */
    inline void set_color(const QColor& qc){
        plot_color = qc;
    }


    /**
     * @brief set_range_reference this plot will display its values in the same ranges as p
     * @param p
     */
    void set_range_reference(Plottable* p);

    inline void set_axis_policy(plot_axis_policy p){
        pap = p;
    }

    /**
     * @brief fix_plot_range display values between given ranges
     * @param rx x range value
     * @param ry y range value
     */
    inline void fix_plot_range(const range& rx,const range& ry){
        pap = given_range;
        x_range = rx;
        y_range = ry;
    }

    /**
     * @brief fix_plot_in_rect display values in a rect centered at (x,y) with half width w and half height h
     * @param x center pos x
     * @param y center pos y
     * @param w half width
     * @param h (OPTIONNAL) half height, it not given, take width value (makes a square)
     */
    inline void fix_plot_in_rect(float x,float y,float w,float h = -1.f){
        if (h < 0.f)
            h = w;
        pap = given_range;
        x_range = {x-w,x+w};
        y_range = {y-h,y+h};
    }


    bool illustrative;

    /**
     * @brief get_x_range get range x to plot values between
     * @return
     */
    inline range get_x_range() const{
        if (range_reference == nullptr)
            return x_range;
        return range_reference->get_x_range();
    }

    /**
     * @brief get_x_range get range y to plot values between
     * @return
     */
    inline range get_y_range() const{
        if (range_reference == nullptr)
            return y_range;
        return range_reference->get_y_range();
    }

    /**
     * @brief get_type get type of the plot
     * @return
     */
    virtual plot_type get_type() const = 0;

    inline bool is_in_group() const {
        return pg != nullptr;
    }

    inline void set_dynamic() {
        dynamic = true;
    }

protected:

    bool dynamic;

    PlotGroup* pg;
    friend class PlotGroup;

    virtual void init_stat_display(PlotFrame*,PlotLayer*,QVBoxLayout*,const StatList&) const {}

    friend class PlotLayer;
    friend class GridLayer;
    friend class StatDisplayer;

    std::string x_label = "x";
    std::string y_label = "y";

    QColor plot_color;

    void draw_axis(frame_draw_object&, const std::pair<range, range>& ranges) const;

    virtual void init() {}

    virtual void call_draw_axis(frame_draw_object&) const = 0;

    virtual void plot(frame_draw_object&) = 0;

    virtual void compute_values(const frame_info& fi) = 0;
    virtual void compute_value_range(const frame_info& fi) = 0;

    range x_range;
    range y_range;

    Plottable* range_reference = nullptr;

    plot_axis_policy pap;

private:
    static constexpr int AXIS_DISTANCE_BETWEEN = 20;
    static constexpr int AXIS_SIZE_BAR = 5;
    static constexpr int AXIS_PRINT_NB_EVERY = 3;
};

}

#endif // PLOTTABLE_H
