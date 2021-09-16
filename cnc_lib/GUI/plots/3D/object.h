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
    Object(Primitive* p,Material& M);
    ~Object();

    inline Primitive* get_primitive() {
        return m_p;
    }

    inline void init() {
        auto f = GLWrapper::get_GL_functions();
        f->glUseProgram(m_M.getShaderId());
        m_M.init();
        m_p->init(m_M);
    }

    inline void draw(const mat4& view) {
        m_M.loadProjectors(localProj,view);
        m_M.loadCustomUniforms();
        m_p->onDraw(m_M);
    }

    void loadProjections(shader_id s);

private:
    Primitive* m_p = nullptr;
    Material& m_M;
    mat4 localProj;
};

}

}
#endif
#endif // OBJECT_H
