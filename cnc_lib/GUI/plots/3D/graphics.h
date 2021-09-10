#ifndef GRAPHICS_H
#define GRAPHICS_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include <QVector3D>
#include <QMatrix4x4>
#include <GL/gl.h>
#include "color.h"

namespace cnc {
namespace graphics {

using vec3 = QVector3D;
using loc = GLuint;
using shader_id = GLint;
using index = GLushort;
using col = Color;
using mat4 = QMatrix4x4;


}
}


#endif // GRAPHICS_H
