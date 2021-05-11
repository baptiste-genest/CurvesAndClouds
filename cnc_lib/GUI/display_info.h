/**
  * @file display_info.h small structs used to transmit info to plots
  */
#ifndef DISPLAY_INFO_H
#define DISPLAY_INFO_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "plots/plot_policy.h"
#include <QPainter>

namespace cnc {

struct frame_info {
    uint width;
    uint height;
};

/**
 * @brief The frame_info struct defines the position and size of a frame
 */
struct frame_draw_object {
    QPainter& painter;
    frame_info fi;
};

}
#endif // DISPLAY_INFO_H
