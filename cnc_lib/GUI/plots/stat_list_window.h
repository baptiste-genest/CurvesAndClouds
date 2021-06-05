/**
  * @file stat_list_window.h small class that is the pop to choose data options
  */
#ifndef STAT_LIST_WINDOW_H
#define STAT_LIST_WINDOW_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "QWidget"
#include "QCheckBox"
#include "QVBoxLayout"
#include "QString"
#include "QLabel"
#include "cnc_error.h"
#include "cnc_types.h"
#include "plot_frame.h"
#include "stat_window.h"
#include "plottable.h"
#include "QPushButton"

namespace cnc {

namespace stat_prop {
    const std::vector<std::string> types_name = {
        "1D scalar function",
        "2D curve",
        "Scalar field",
        "Vector field",
        "Text",
        "Figure",
        "Point cloud",
    };

    constexpr static int INTEGRAL = 0;
    constexpr static int FIRST_DER = 1;
    constexpr static int SCND_DER = 2;
    constexpr static int PCA = 0;
    constexpr static int MEAN = 1;
    constexpr static int GRAD = 0;
    constexpr static int ROT_NORM = 0;
    constexpr static int DIV = 1;

    const std::vector<std::vector<std::string>> prop_names = {
        {//function plot
         "integral over domain",
         "derivative average",
         "second derivative average"
        },{//d2_curve
           "null"
        },{//scalar field
           "gradient"
        },{//vector field
           "rot norm",
           "div"
        },{//text
           "null"
        },{//figure
           "null"
        },{//point cloud
           "pricipal componants axis",
           "mean"
        }
    };

}

class StatListWindow : public QWidget
{
    Q_OBJECT
public:
    StatListWindow(PlotFrame*);
private:
    std::vector<std::vector<QCheckBox*>> boxes;
    PlotFrame* ref;

private slots:
    void exit_slot();
};

}
#endif // STAT_LIST_WINDOW_H
