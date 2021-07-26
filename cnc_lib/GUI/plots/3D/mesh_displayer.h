#ifndef MESH_DISPLAYER_H
#define MESH_DISPLAYER_H

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <GL/glu.h>
#include <GL/gl.h>
#include "../plottable.h"

namespace cnc {

class MeshDisplayer : public Plottable, public QOpenGLWidget,public QOpenGLFunctions
{
public:
    MeshDisplayer();
    virtual ~MeshDisplayer();

    plot_type get_type() const override {return plot_type::boolean_field;}

private:
    bool ok = false;
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int,int) override;

    void call_draw_axis(frame_draw_object&) const override {}
    void plot(frame_draw_object&) override {paintGL();}
    void compute_values(const frame_info&) override {}
    void compute_value_range(const frame_info&) override {}

    /*
    QOpenGLContext* m_open_gl_context;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program;
    QOpenGLShader *m_shader;
    QOpenGLTexture *m_texture;
    QOpenGLFunctions *m_openGLFunctions;
    */
};

}
#endif // MESH_DISPLAYER_H
