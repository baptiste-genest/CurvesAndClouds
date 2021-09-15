#ifdef CNC_OPENGL
#include "geometryengine.h"

cnc::GeometryEngine::GeometryEngine(algo::geometry::SimpleGLMesh *M) :
    indexBuf(QOpenGLBuffer::IndexBuffer),mesh(M)
{
}

void cnc::GeometryEngine::paint_mesh(QOpenGLShaderProgram *program)
{
    arrayBuf.bind();
    if (dynamic || must_update){
        must_update = false;
        arrayBuf.allocate(mesh->get_points().data(),mesh->get_nb_vertices()*sizeof(algo::geometry::VertexInfo));
    }
    indexBuf.bind();

    int offset = 0;

    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(algo::geometry::VertexInfo));

    offset += sizeof(QVector3D);

    int normalLocation = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(algo::geometry::VertexInfo));

    offset += sizeof(QVector3D);

    int colorLocation = program->attributeLocation("a_color");
    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(algo::geometry::VertexInfo));

    offset += sizeof(QVector3D);

    int valueLocation = program->attributeLocation("a_value");
    program->enableAttributeArray(valueLocation);
#if PRECISION == DOUBLE
    program->setAttributeBuffer(valueLocation, GL_DOUBLE, offset, 1, sizeof(algo::geometry::VertexInfo));
#else
    program->setAttributeBuffer(valueLocation, GL_FLOAT, offset, 1, sizeof(algo::geometry::VertexInfo));
#endif

    testBuf.bind();

    glDrawElements(GL_TRIANGLES, nb_index, GL_UNSIGNED_INT, nullptr);
}

void cnc::GeometryEngine::initialize()
{
    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();
    testBuf.create();

    arrayBuf.bind();
    arrayBuf.allocate(mesh->get_points().data(),mesh->get_nb_vertices()*sizeof(algo::geometry::VertexInfo));

    indexBuf.bind();
    nb_index = mesh->get_nb_faces()*sizeof(uint)*3;
    indexBuf.allocate(mesh->get_faces().data(),nb_index);

}

#endif
