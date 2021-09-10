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
    Primitive::onDraw(view);
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
