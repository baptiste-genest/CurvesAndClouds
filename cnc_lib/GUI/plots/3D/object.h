#ifndef OBJECT_H
#define OBJECT_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
#ifdef CNC_OPENGL


#include "primitives/primitive.h"
#include "primitives/quad.h"


namespace cnc {

namespace graphics {

class Object
{
public:
    Object(Primitive* p);
    ~Object();

    inline Primitive* get_primitive() {
        return m_p;
    }

    inline void initPrimitive() {
        m_p->init(M);
    }

private:
    Primitive* m_p = nullptr;
    Material M;
};

}

}
#endif
#endif // OBJECT_H
