#include "primitive.h"

cnc::graphics::Primitive::Primitive()
{
    local_transform.setToIdentity();
    std::string prefix = "/home/eulerson/Curves-Clouds/cnc_lib/GUI/plots/3D/shaders/";
    vertex_shader_loc = prefix + "basic_vertex_shader.glsl";
    frag_shader_loc = prefix + "basic_fragment_shader.glsl";
}

void cnc::graphics::Primitive::onDraw(const cnc::graphics::mat4 &view)
{
    if (!initialized)
        return;
    auto f = GLWrapper::get_GL_functions();
    // activer le shader des triangles
    f->glUseProgram(m_shaderId);

    loadUniforms(view);

    loadBuffers();


    // activer et lier le buffer contenant les indices des triangles
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);

    // dessiner les triangles
    f->glDrawElements(GL_TRIANGLES, TRIANGLE_COUNT * 3, GL_UNSIGNED_SHORT, 0);

    // désactiver les buffers qui ne sont plus utilisés
    f->glDisableVertexAttribArray(m_glVertexLoc);
    f->glDisableVertexAttribArray(m_glColorLoc);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    f->glUseProgram(0);
}

void cnc::graphics::Primitive::getUniformsLoc()
{
    auto f = GLWrapper::get_GL_functions();
    m_localMatrixLoc = f->glGetUniformLocation(m_shaderId, "local_proj");
    m_viewMatrixLoc = f->glGetUniformLocation(m_shaderId, "view_proj");
}

void cnc::graphics::Primitive::getAttributesLoc()
{
    auto f = GLWrapper::get_GL_functions();
    m_glVertexLoc = f->glGetAttribLocation(m_shaderId, "glVertex");
    m_glColorLoc = f->glGetAttribLocation(m_shaderId, "glColor");
}

void cnc::graphics::Primitive::loadBuffers()
{
    auto f = GLWrapper::get_GL_functions();
    // activer et lier le buffer contenant les coordonnées
    f->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
    f->glEnableVertexAttribArray(m_glVertexLoc);
    f->glVertexAttribPointer(m_glVertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // activer et lier le buffer contenant les couleurs
    f->glBindBuffer(GL_ARRAY_BUFFER, m_ColorBufferId);
    f->glEnableVertexAttribArray(m_glColorLoc);
    f->glVertexAttribPointer(m_glColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void cnc::graphics::Primitive::loadUniforms(const mat4& view)
{
    auto f = GLWrapper::get_GL_functions();
    // fournir la matrice au shader
    f->glUniformMatrix4fv(m_viewMatrixLoc,1,GL_FALSE,view.data());
    f->glUniformMatrix4fv(m_localMatrixLoc,1,GL_FALSE,local_transform.data());

}
