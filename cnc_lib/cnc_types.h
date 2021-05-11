/**
  * @file cnc_types.h defines basic cnc_types
  */
#ifndef CNC_TYPES_H
#define CNC_TYPES_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <utility>
#include <iostream>

typedef unsigned int uint;

/**
 * @brief The namespace to use all the functions of Curves&Clouds
 */
namespace cnc {

/**
 * @brief A pair of float
 */
typedef std::pair<float,float> range;

enum axis {
    x_axis,
    y_axis
};
}
#endif // CNC_TYPES_H
