#ifndef UNIFORM_LIGHT_H
#define UNIFORM_LIGHT_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef CNC_OPENGL

#include "light_source.h"
#include "../graphics.h"

namespace cnc {

namespace graphics {

class UniformLight : public LightSource
{
public:
    UniformLight();
private:
    vec3 position;

};


}

}

#endif

#endif // UNIFORM_LIGHT_H
