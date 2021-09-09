#include "scene_viewer.h"

cnc::SceneViewer::SceneViewer(QWidget *parent) : QOpenGLWidget(parent)
{
    m_scene = new graphics::Scene();
}

cnc::SceneViewer::~SceneViewer()
{
    delete m_scene;
}

void cnc::SceneViewer::initializeGL()
{
    m_scene->init();
}

void cnc::SceneViewer::resizeGL(int w, int h)
{
    m_scene->resize(w,h);
}


void cnc::SceneViewer::paintGL()
{
    m_scene->draw();
}
