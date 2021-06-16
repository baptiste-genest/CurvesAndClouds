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
    GUI/algo/mutablevalue.cpp \
    GUI/algo/mutablevaluecursor.cpp \
    GUI/algo/mutablevalueticker.cpp \
    GUI/plots/boolean_field.cpp \
    GUI/plots/colormap.cpp \
    GUI/plots/complex_function_plot.cpp \
    GUI/plots/curves.cpp \
    GUI/plots/d2_curve.cpp \
    GUI/plots/field.cpp \
    GUI/plots/figure.cpp \
    GUI/plots/formula_tex.cpp \
    GUI/plots/functionplot.cpp \
    GUI/plots/grid_layer.cpp \
    GUI/plots/level_curve.cpp \
    GUI/plots/plot_frame.cpp \
    GUI/plots/plot_group.cpp \
    GUI/plots/plot_layer.cpp \
    GUI/plots/plot_stat_display.cpp \
    GUI/plots/plot_tab.cpp \
    GUI/plots/plottable.cpp \
    GUI/plots/point_cloud.cpp \
    GUI/plots/scalar_field.cpp \
    GUI/plots/stat_list_window.cpp \
    GUI/plots/stat_window.cpp \
    GUI/plots/trajectory.cpp \
    GUI/plots/vector_field.cpp \
    GUI/plotwindow.cpp \
    GUI/ColorWheel.cpp \
    core/algo/algo.cpp \
    core/algo/bit_operations.cpp \
    core/algo/calculus/calculus.cpp \
    core/algo/calculus/differential_equations.cpp \
    core/algo/calculus/finite_differences.cpp \
    core/algo/calculus/fourier.cpp \
    core/algo/calculus/integration_scheme.cpp \
    core/algo/calculus/lagrange_interpolator.cpp \
    core/algo/calculus/optimization.cpp \
    core/algo/calculus/vector_calculus.cpp \
    core/algo/geometry/geometry.cpp \
    core/algo/geometry/halfedge.cpp \
    core/algo/geometry/mesh.cpp \
    core/algo/geometry/quadtree.cpp \
    core/algo/geometry/quadtreemesher.cpp \
    core/algo/geometry/triangle.cpp \
    core/algo/inout.cpp \
    core/algo/lin_alg.cpp \
    core/algo/matrix_functions.cpp \
    core/algo/statistics/random_var.cpp \
    core/algo/statistics/statistics.cpp

HEADERS += \
    GUI/algo/algo_gui.h \
    GUI/algo/mutablevalue.h \
    GUI/algo/mutablevaluecursor.h \
    GUI/algo/mutablevalueticker.h \
    GUI/display_info.h \
    GUI/plot_frame.h \
    GUI/plot_layer.h \
    GUI/plot_policy.h \
    GUI/plot_tab.h \
    GUI/plots/boolean_field.h \
    GUI/plots/colormap.h \
    GUI/plots/complex_function_plot.h \
    GUI/plots/curves.h \
    GUI/plots/d2_curve.h \
    GUI/plots/field.h \
    GUI/plots/figure.h \
    GUI/plots/formula_tex.h \
    GUI/plots/functionplot.h \
    GUI/plots/grid_layer.h \
    GUI/plots/level_curve.h \
    GUI/plots/plot_frame.h \
    GUI/plots/plot_group.h \
    GUI/plots/plot_layer.h \
    GUI/plots/plot_policy.h \
    GUI/plots/plot_tab.h \
    GUI/plots/plottable.h \
    GUI/plots/plotwindow.h \
    GUI/plots/point_cloud.h \
    GUI/plots/scalar_field.h \
    GUI/plots/stat_list_window.h \
    GUI/plots/stat_window.h \
    GUI/plots/trajectory.h \
    GUI/plots/vector_field.h \
    GUI/plotwindow.h \
    GUI/ColorWheel.h \
    cnc_error.h \
    cnc_types.h \
    core/algo/algo.h \
    core/algo/bit_operations.h \
    core/algo/calculus/calculus.h \
    core/algo/calculus/differential_equations.h \
    core/algo/calculus/finite_differences.h \
    core/algo/calculus/finite_elements/finite_elements.h \
    core/algo/calculus/fourier.h \
    core/algo/calculus/integration_scheme.h \
    core/algo/calculus/lagrange_interpolator.h \
    core/algo/calculus/optimization.h \
    core/algo/calculus/vector_calculus.h \
    core/algo/geometry/geometry.h \
    core/algo/geometry/halfedge.h \
    core/algo/geometry/mesh.h \
    core/algo/geometry/quadtree.h \
    core/algo/geometry/quadtreemesher.h \
    core/algo/geometry/triangle.h \
    core/algo/inout.h \
    core/algo/lin_alg.h \
    core/algo/matrix_functions.h \
    core/algo/statistics/random_var.h \
    core/algo/statistics/statistics.h \
    core/matrix_algo.h \
    curvesandcloudCore.h \
    core/matrix.h \
    core/matrix_errors.h \
    curvesandcloud.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
