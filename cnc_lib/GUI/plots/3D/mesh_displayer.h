#ifndef MESH_DISPLAYER_H
#define MESH_DISPLAYER_H

#if CNC_OPENGL == TRUE

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QResizeEvent>
#include <QBasicTimer>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include "core/algo/inout.h"

#include "core/algo/geometry/simpleglmesh.h"
#include "geometryengine.h"
#include "examples/mesh_bank.h"

/*
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
*/

namespace cnc {

enum mesh_color_mode {
    white,
    interpolate_value,
    given_color,
};

class MeshDisplayer : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT;
public:
    MeshDisplayer(QWidget* parent);

    void load_mesh_from_obj(const std::string& filename,float scale = 1.f);

    inline void load_mesh(bank_mesh_names name) {
        load_mesh_from_obj(data_prefix + MESH_BANK[name].filename,MESH_BANK[name].scale_factor);
    }

    virtual ~MeshDisplayer();

    inline algo::geometry::SimpleGLMesh* get_mesh() {
        if (mesh == nullptr)
            throw Cnc_error("Mesh isn't build yet");
        return mesh;
    }

    inline algo::geometry::SimpleGLMesh* set_mesh(uint nb_vertex,uint nb_faces) {
        mesh = new algo::geometry::SimpleGLMesh(nb_vertex,nb_faces);
        GE = new GeometryEngine(mesh);
        return mesh;
    }

    inline void set_ambient_light(bool p){
        ambient = p;
    }

    inline void set_specular_reflection(bool p){
        specular = p;
    }

    inline void set_color_mode(mesh_color_mode m) {
        mcm = m;
    }

    inline GeometryEngine* get_geometry_engine() {
        return GE;
    }

    inline void set_dynamic(bool d) {
        GE->declare_dynamic(d);
    }

    inline void set_light_pos(const QVector3D& p){
        light_pos = p;
    }
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w,int h) override;
    virtual void paintGL() override;

    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
    virtual void timerEvent(QTimerEvent* e) override;

    void initShaders();
private:

    QVector3D light_pos;
    algo::geometry::SimpleGLMesh* mesh = nullptr;

    mesh_color_mode mcm = white;
    bool ambient = false;
    bool specular = false;

    QOpenGLShaderProgram program;
    GeometryEngine* GE = nullptr;
    QMatrix4x4 projection;

    QBasicTimer timer;

    QVector3D camera_pos;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;

    const std::string vertex_shader_loc = "/home/eulerson/Curves-Clouds/cnc_lib/GUI/plots/3D/vshader.glsl";
    const std::string fragment_shader_loc = "/home/eulerson/Curves-Clouds/cnc_lib/GUI/plots/3D/fshader.glsl";

signals:
    void gl_initialized();

};

}

#endif
#endif // MESH_DISPLAYER_H
