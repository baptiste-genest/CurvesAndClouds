#ifndef CNC_LIB_GLOBAL_H
#define CNC_LIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CNC_LIB_LIBRARY)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#endif // CNC_LIB_GLOBAL_H
