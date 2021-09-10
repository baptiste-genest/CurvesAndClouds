#include "scene.h"
#include "primitives/quad.h"

cnc::graphics::Scene::Scene()
{

}

cnc::graphics::Scene::~Scene()
{
    for (Object* o : m_objects)
        delete o;
    m_objects.clear();
}

void cnc::graphics::Scene::init()
{
    auto f = GLWrapper::get_GL_functions();
    f->glClearColor(0,0,0,1);

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);

    for (Object* o : m_objects)
        o->get_primitive()->init();

    m_view.setToIdentity();
}

void cnc::graphics::Scene::resize(int w, int h)
{
    auto f = GLWrapper::get_GL_functions();
    f->glViewport(0,0,w,h);

    /*
    float aspect = float(w) / float(h ? h : 1);
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;
    m_view.perspective(fov, aspect, zNear, zFar);
    */
}

void cnc::graphics::Scene::draw()
{
    auto f = GLWrapper::get_GL_functions();
    // effacer l'écran
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Object* o : m_objects)
        o->get_primitive()->onDraw(m_view);
}
