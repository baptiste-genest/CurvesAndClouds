#include "quad.h"
#ifdef CNC_OPENGL
#include <QOpenGLContext>

cnc::graphics::Quad::Quad(const std::vector<vec3> &pos)
    : m_vertices(pos)
{
    m_indexArray = std::vector<index>({0,1,2,1,3,2});
    vec3 n1 = QVector3D::crossProduct(pos[1]-pos[0],pos[2]-pos[0]).normalized();
    vec3 n2 = QVector3D::crossProduct(pos[1]-pos[3],pos[2]-pos[3]).normalized();
    vec3 avg = (n1+n2).normalized();
    m_normals.resize(4);
    m_normals[0] = n1;
    m_normals[1] = avg;
    m_normals[2] = avg;
    m_normals[3] = n2;
}

void cnc::graphics::Quad::init(const Material &M)
{
    TRIANGLE_COUNT = 2;
    m_glVertexLoc = M.getAttributeLoc("vertex");
    m_normalLoc =  M.getAttributeLoc("normal");
    m_VertexBufferId = GLWrapper::create_vbo<vec3>(m_vertices,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
    m_normalBufferId = GLWrapper::create_vbo<vec3>(m_normals,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
    m_IndexBufferId = GLWrapper::create_vbo<index>(m_indexArray,GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);
}

void cnc::graphics::Quad::onDraw(const Material &M)
{
    Primitive::onDraw(M);
}

void cnc::graphics::Quad::loadBuffers()
{
    auto f = GLWrapper::get_GL_functions();

    f->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
    f->glEnableVertexAttribArray(m_glVertexLoc);
    f->glVertexAttribPointer(m_glVertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    f->glBindBuffer(GL_ARRAY_BUFFER,m_normalBufferId);
    f->glEnableVertexAttribArray(m_normalLoc);
    f->glVertexAttribPointer(m_normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

#endif
