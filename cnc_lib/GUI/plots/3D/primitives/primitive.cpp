#include "primitive.h"
#ifdef CNC_OPENGL

cnc::graphics::Primitive::Primitive()
{
}

void cnc::graphics::Primitive::onDraw(const Material& M)
{
    if (!initialized)
        return;
    auto f = GLWrapper::get_GL_functions();
    // activer le shader des triangles

    loadBuffers();


    // activer et lier le buffer contenant les indices des triangles
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);

    // dessiner les triangles
    f->glDrawElements(GL_TRIANGLES, TRIANGLE_COUNT * 3, GL_UNSIGNED_SHORT, 0);

    // désactiver les buffers qui ne sont plus utilisés
    f->glDisableVertexAttribArray(m_glVertexLoc);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    f->glUseProgram(0);
}

void cnc::graphics::Primitive::getVertexLoc(const Material &M)
{
    auto f = GLWrapper::get_GL_functions();
    f->glGetAttribLocation(M.getShaderId(),"");
    //m_glVertexLoc = f;
}


#endif
