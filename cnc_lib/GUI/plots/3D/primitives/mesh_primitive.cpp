#include "mesh_primitive.h"

#ifdef CNC_OPENGL


cnc::graphics::Mesh::Mesh(const std::vector<vec3> &pos, const std::vector<vec3> &normals, const std::vector<index> faces)
    :
      m_vertices(pos),
      m_normals(normals),
      m_indexArray(faces)
{

}

void cnc::graphics::Mesh::init(const cnc::graphics::Material &M)
{
    m_normalLoc = M.getVertexLoc();

    m_VertexBufferId = GLWrapper::create_vbo<vec3>(m_vertices,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
    m_normalBufferId = GLWrapper::create_vbo<vec3>(m_normals,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
    m_IndexBufferId = GLWrapper::create_vbo<index>(m_indexArray,GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);
}

void cnc::graphics::Mesh::loadBuffers()
{
    auto f = GLWrapper::get_GL_functions();

    f->glBindBuffer(GL_ARRAY_BUFFER,m_normalBufferId);
    f->glEnableVertexAttribArray(m_normalLoc);
    f->glVertexAttribPointer(m_normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    f->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
    f->glEnableVertexAttribArray(m_glVertexLoc);
    f->glVertexAttribPointer(m_glVertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

/*
void cnc::graphics::Mesh::add_vertex(const vec3 &x)
{
    m_vertices.push_back(x);
}

void cnc::graphics::Mesh::add_face(index i1, index i2, index i3)
{
    m_varray.push_back(i1);
    m_varray.push_back(i2);
    m_varray.push_back(i3);
}
*/

#endif
