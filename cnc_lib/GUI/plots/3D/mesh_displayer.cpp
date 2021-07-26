#include "mesh_displayer.h"

cnc::MeshDisplayer::MeshDisplayer()
    /*:
    m_program(0),
    m_shader(0),
    m_texture(0)
   */
    : QOpenGLWidget(nullptr)
{
    //MeshDisplayer::initializeGL();
}


void cnc::MeshDisplayer::initializeGL()
{
    ok = true;
    initializeOpenGLFunctions();
    glClearColor(0.f,0.f,0.f,1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void cnc::MeshDisplayer::paintGL()
{
    std::cout << ok << std::endl;
    if (!ok)
        return;
        //initializeGL();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void cnc::MeshDisplayer::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


cnc::MeshDisplayer::~MeshDisplayer()
{
    /*
    makeCurrent();

    delete m_texture;
    delete m_shader;
    delete m_program;

    m_vbo.destroy();
    m_vao.destroy();

    doneCurrent();
    */
}
