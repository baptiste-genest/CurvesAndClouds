#ifndef PRIMITIVE_TRIANGLE_H
#define PRIMITIVE_TRIANGLE_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include "primitive.h"

namespace cnc {

namespace graphics {

/*
class Triangle : public Primitive
{
public:
    Triangle();

    virtual void onDraw(const mat4& mvp) override;
};
*/

}

}

#endif // TRIANGLE_H
