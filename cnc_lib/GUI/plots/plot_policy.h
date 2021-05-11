#ifndef PLOT_POLICY_H
#define PLOT_POLICY_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

namespace cnc {

enum plot_axis_policy {
    min_max_range,
    given_range,
    same_than_x_range,
    same_than_y_range,
    square_max_range
};

enum color_policy {
    min_max,
    from_zero
};

}
#endif // PLOT_POLICY_H
