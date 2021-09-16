#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef CNC_OPENGL
#include <QMatrix4x4>
#include <GL/gl.h>
#include "../color.h"
#include "../graphics.h"
#include "../GLWrapper.h"
#include "../material.h"

namespace cnc {

namespace graphics {

class Primitive {
public:
    Primitive();
    virtual ~Primitive() {}

    virtual void init(const Material& M) = 0;
    virtual void onDraw(const Material& M);

protected:
    virtual void loadBuffers() = 0;
    void getVertexLoc(const Material& M);

    GLint m_glVertexLoc;

    // Buffers
    loc m_VertexBufferId;
    loc m_IndexBufferId;

    int TRIANGLE_COUNT = 0;

    bool initialized = false;

};


}

}

#endif

#endif // PRIMITIVE_H
