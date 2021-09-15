#include "scene.h"
#include "primitives/quad.h"

#include "core/time_handling.h"

cnc::graphics::Scene::Scene()
{
    cam = new Camera();
}

cnc::graphics::Scene::~Scene()
{
    for (Object* o : m_objects)
        delete o;
    m_objects.clear();
    delete cam;
}

void cnc::graphics::Scene::init()
{
    auto f = GLWrapper::get_GL_functions();
    f->glClearColor(0.3,0.3,0.4,1);

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);

    for (Object* o : m_objects)
        o->get_primitive()->init();
}

void cnc::graphics::Scene::resize(int w, int h)
{
    auto f = GLWrapper::get_GL_functions();
    f->glViewport(0,0,w,h);
}

void cnc::graphics::Scene::draw()
{
    auto f = GLWrapper::get_GL_functions();

    // effacer l'écran
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 View = cam->getViewMatrix();
    for (Object* o : m_objects)
        o->get_primitive()->onDraw(View);
}
