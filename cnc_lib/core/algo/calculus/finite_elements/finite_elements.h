#ifndef FINITE_ELEMENTS_H
#define FINITE_ELEMENTS_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "../../geometry/quadtreemesher.h"

#endif // FINITE_ELEMENTS_H
