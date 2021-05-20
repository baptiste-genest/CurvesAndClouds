QT += gui core widgets network

TEMPLATE = lib
DEFINES += CNC_LIB
DEFINES += PRECISION DOUBLE
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/algo/algo_gui.cpp \
    GUI/plots/colormap.cpp \
    GUI/plots/curves.cpp \
    GUI/plots/d2_curve.cpp \
    GUI/plots/field.cpp \
    GUI/plots/figure.cpp \
    GUI/plots/formula_tex.cpp \
    GUI/plots/function_plot.cpp \
    GUI/plots/level_curve.cpp \
    GUI/plots/plot_frame.cpp \
    GUI/plots/plot_layer.cpp \
    GUI/plots/plot_stat_display.cpp \
    GUI/plots/plot_tab.cpp \
    GUI/plots/plottable.cpp \
    GUI/plots/point_cloud.cpp \
    GUI/plots/scalar_field.cpp \
    GUI/plots/stat_list_window.cpp \
    GUI/plots/stat_window.cpp \
    GUI/plots/vector_field.cpp \
    GUI/plotwindow.cpp \
    core/algo/algo.cpp \
    core/algo/bit_operations.cpp \
    core/algo/calculus/calculus.cpp \
    core/algo/calculus/differential_equations.cpp \
    core/algo/calculus/finite_differences.cpp \
    core/algo/calculus/finite_elements/mesh.cpp \
    core/algo/calculus/finite_elements/quadtree.cpp \
    core/algo/calculus/finite_elements/quadtreemesher.cpp \
    core/algo/calculus/finite_elements/triangle.cpp \
    core/algo/calculus/fourier.cpp \
    core/algo/calculus/integration_scheme.cpp \
    core/algo/calculus/lagrange_interpolator.cpp \
    core/algo/calculus/optimization.cpp \
    core/algo/calculus/vector_calculus.cpp \
    core/algo/inout.cpp \
    core/algo/statistics/random_var.cpp \
    core/algo/statistics/statistics.cpp

HEADERS += \
    GUI/algo/algo_gui.h \
    GUI/display_info.h \
    GUI/plot_frame.h \
    GUI/plot_layer.h \
    GUI/plot_policy.h \
    GUI/plot_tab.h \
    GUI/plots/colormap.h \
    GUI/plots/curves.h \
    GUI/plots/d2_curve.h \
    GUI/plots/field.h \
    GUI/plots/figure.h \
    GUI/plots/formula_tex.h \
    GUI/plots/function_plot.h \
    GUI/plots/level_curve.h \
    GUI/plots/plot_frame.h \
    GUI/plots/plot_layer.h \
    GUI/plots/plot_policy.h \
    GUI/plots/plot_tab.h \
    GUI/plots/plottable.h \
    GUI/plots/plotwindow.h \
    GUI/plots/point_cloud.h \
    GUI/plots/scalar_field.h \
    GUI/plots/stat_list_window.h \
    GUI/plots/stat_window.h \
    GUI/plots/vector_field.h \
    GUI/plotwindow.h \
    cnc_error.h \
    cnc_types.h \
    core/algo/algo.h \
    core/algo/bit_operations.h \
    core/algo/calculus/calculus.h \
    core/algo/calculus/differential_equations.h \
    core/algo/calculus/finite_differences.h \
    core/algo/calculus/finite_elements/finite_elements.h \
    core/algo/calculus/finite_elements/mesh.h \
    core/algo/calculus/finite_elements/quadtree.h \
    core/algo/calculus/finite_elements/quadtreemesher.h \
    core/algo/calculus/finite_elements/triangle.h \
    core/algo/calculus/fourier.h \
    core/algo/calculus/integration_scheme.h \
    core/algo/calculus/lagrange_interpolator.h \
    core/algo/calculus/optimization.h \
    core/algo/calculus/vector_calculus.h \
    core/algo/inout.h \
    core/algo/lin_alg.h \
    core/algo/statistics/random_var.h \
    core/algo/statistics/statistics.h \
    curvesandcloudCore.h \
    core/matrix.h \
    core/matrix_errors.h \
    curvesandcloud.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
