#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H


#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef CNC_OPENGL
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <iostream>

#include "core/algo/geometry/simpleglmesh.h"

namespace cnc {

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine(algo::geometry::SimpleGLMesh* M);
    void paint_mesh(QOpenGLShaderProgram* program);

    void initialize();

    inline void declare_update_need() {
        must_update = true;
    }

    inline void declare_dynamic(bool d = true) {
        dynamic = d;
    }

private:
    bool dynamic = false;
    bool must_update = false;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLBuffer testBuf;
    algo::geometry::SimpleGLMesh* mesh;
    int nb_index = 0;
};

}
#endif

#endif // GEOMETRYENGINE_H
