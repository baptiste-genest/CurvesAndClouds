#include "quad.h"
#include <QOpenGLContext>

cnc::graphics::Quad::Quad(const std::vector<vec3>& points)
    : Primitive()
{
    Primitive::m_points = points;

    m_index = {
                0,1,2,
                1,2,3
              };

    m_colors = {
        col(1.f,0,0),
        col(0,1.f,0),
        col(0,0,1.f),
        col(1.f,0,1.f)
    };

    TRIANGLE_COUNT = 2;
}

void cnc::graphics::Quad::onDraw(const cnc::graphics::mat4 &view)
{
    if (!initialized)
        return;
    auto f = GLWrapper::get_GL_functions();
    // activer le shader des triangles
    f->glUseProgram(m_shaderId);

    // fournir la matrice au shader
    f->glUniformMatrix4fv(m_viewMatrixLoc,1,GL_FALSE,view.data());
    f->glUniformMatrix4fv(m_localMatrixLoc,1,GL_FALSE,local_transform.data());

    // activer et lier le buffer contenant les coordonnées
    f->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
    f->glEnableVertexAttribArray(m_glVertexLoc);
    f->glVertexAttribPointer(m_glVertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // activer et lier le buffer contenant les couleurs
    f->glBindBuffer(GL_ARRAY_BUFFER, m_ColorBufferId);
    f->glEnableVertexAttribArray(m_glColorLoc);
    f->glVertexAttribPointer(m_glColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);

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

void cnc::graphics::Quad::init()
{
    m_shaderId = GLWrapper::makeShaderProgram(Primitive::vertex_shader_loc,Primitive::frag_shader_loc);

    Primitive::getUniformsLoc();
    Primitive::getAttributesLoc();

    m_VertexBufferId = GLWrapper::create_vbo<vec3>(m_points,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
    m_ColorBufferId = GLWrapper::create_vbo<col>(m_colors,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
    m_IndexBufferId = GLWrapper::create_vbo<index>(m_index,GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);

    initialized = true;
}
