#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef CNC_OPENGL


namespace cnc {

namespace graphics {

class LightSource
{
public:
    LightSource();

};

}

}

#endif


#endif // *LIGHT_SOURCE_H
