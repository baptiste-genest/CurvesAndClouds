#include "mesh_displayer.h"

cnc::MeshDisplayer::MeshDisplayer(QWidget* parent)
    : QOpenGLWidget(parent)
{
    camera_pos = QVector3D(0,0,5);
}

void cnc::MeshDisplayer::load_mesh_from_obj(const std::string &filename,float scale)
{
    mesh = new algo::geometry::SimpleGLMesh(algo::import_mesh_from_obj(filename,scale));
    GE = new GeometryEngine(mesh);
}

void cnc::MeshDisplayer::initializeGL()
{
    if (mesh == nullptr)
        throw Cnc_error("No mesh loaded in Mesh Displayer!");
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);

    initShaders();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    GE->initialize();

    timer.start(12,this);


    emit gl_initialized();

}

void cnc::MeshDisplayer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrix;
    matrix.translate(-camera_pos);
    matrix.rotate(rotation);

    program.setUniformValue("mvp_matrix", projection * matrix);
    program.setUniformValue("cam_pos", rotation.inverted()*camera_pos);
    program.setUniformValue("light_pos", QVector3D(10.,-10.,0.));
    program.setUniformValue("ambient_light", (int)ambient);
    program.setUniformValue("specular", (int)specular);
    program.setUniformValue("color_mode", (int)mcm);

    GE->paint_mesh(&program);
}


void cnc::MeshDisplayer::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex,vertex_shader_loc.data()))
        throw Cnc_error("Couldn't compile vertex shader");

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,fragment_shader_loc.data()))
        throw Cnc_error("Couldn't compile fragment shader");

    if (!program.link())
        throw Cnc_error("Couldn't link shader");

    if (!program.bind())
        throw Cnc_error("Couldn't bind shader");
}

void cnc::MeshDisplayer::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

cnc::MeshDisplayer::~MeshDisplayer()
{
    makeCurrent();
    delete GE;
    delete mesh;
    doneCurrent();
}

void cnc::MeshDisplayer::mousePressEvent(QMouseEvent *e)
{
    mousePressPosition = QVector2D(e->localPos());
}

void cnc::MeshDisplayer::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    qreal acc = diff.length() / 100.0;

    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    angularSpeed += acc;
}

void cnc::MeshDisplayer::timerEvent(QTimerEvent *)
{
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
}
