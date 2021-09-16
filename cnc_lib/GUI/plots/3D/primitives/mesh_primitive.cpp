#include "mesh_primitive.h"

#ifdef CNC_OPENGL


cnc::graphics::Mesh::Mesh(const std::vector<vec3> &pos, const std::vector<vec3> &normals, const std::vector<index> faces)
    :
      m_vertices(pos),
      m_normals(normals),
      m_indexArray(faces)
{

}

cnc::graphics::Mesh::Mesh(const std::string &input_file, float scale)
{
    using namespace algo;
    std::ifstream file(input_file);
    if (!file.is_open())
        throw Cnc_error("Can't open the file: " + input_file);

    std::vector<QVector3D> vpos;
    std::vector<QVector3D> vnormals;
    std::vector<index> faces;

    QVector3D tmp;
    std::string buffer;
    std::vector<std::string> tokens;
    bool quad_mesh = false;
    while (getline(file,buffer)){
        if (buffer.size() == 0)
            continue;
        tokens = split(buffer,' ',true);
        if (tokens[0] == "v"){
            for (uint k = 0;k<3;k++){
                std::stringstream ss(tokens[k+1]);
                ss >> tmp[k];
            }
            vpos.push_back(tmp*scale);
        } else if (tokens[0] == "vn"){
            for (uint k = 0;k<3;k++){
                std::stringstream ss(tokens[k+1]);
                ss >> tmp[k];
            }
            vnormals.push_back(tmp);
        } else if (tokens[0] == "f") {
            bool quad = tokens.size() == 6;
            std::vector<uint> ids;
            for (uint k = 0;k<3;k++){
                ids.push_back(std::stoi(split(tokens[k+1],'/')[0])-1);
                faces.push_back(ids.back());
            }
            if (quad){
                quad_mesh = true;
                ids.push_back(std::stoi(split(tokens[4],'/')[0])-1);
                faces.push_back(ids[2]);
                faces.push_back(ids[3]);
                faces.push_back(ids[0]);
            }
        }
    }
    if (quad_mesh)
        std::cout << "quad detected" << std::endl;
    m_vertices = vpos;
    m_normals = vnormals;
    m_indexArray = faces;
}

void cnc::graphics::Mesh::init(const cnc::graphics::Material &M)
{
    initialized = true;
    TRIANGLE_COUNT = m_indexArray.size()/3;
    m_glVertexLoc = M.getAttributeLoc("vertex");
    m_normalLoc =  M.getAttributeLoc("normal");
    m_VertexBufferId = GLWrapper::create_vbo<vec3>(m_vertices,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
    m_normalBufferId = GLWrapper::create_vbo<vec3>(m_normals,GL_ARRAY_BUFFER,GL_STATIC_DRAW);
    m_IndexBufferId = GLWrapper::create_vbo<index>(m_indexArray,GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW);
}

void cnc::graphics::Mesh::onDraw(const cnc::graphics::Material &M)
{
    Primitive::onDraw(M);
}

void cnc::graphics::Mesh::loadBuffers()
{
    auto f = GLWrapper::get_GL_functions();

    f->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
    f->glEnableVertexAttribArray(m_glVertexLoc);
    f->glVertexAttribPointer(m_glVertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    f->glBindBuffer(GL_ARRAY_BUFFER,m_normalBufferId);
    f->glEnableVertexAttribArray(m_normalLoc);
    f->glVertexAttribPointer(m_normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
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
