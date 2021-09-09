#ifndef OBJECT_H
#define OBJECT_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include "primitives/primitive.h"

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
        m_p->init();
    }

private:
    Primitive* m_p = nullptr;
};

}

}
#endif // OBJECT_H
