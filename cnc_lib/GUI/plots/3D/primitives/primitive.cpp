#include "primitive.h"

cnc::graphics::Primitive::Primitive()
{
    local_transform.setToIdentity();
    std::string prefix = "/home/eulerson/Curves-Clouds/cnc_lib/GUI/plots/3D/shaders/";
    vertex_shader_loc = prefix + "basic_vertex_shader.glsl";
    frag_shader_loc = prefix + "basic_fragment_shader.glsl";
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
