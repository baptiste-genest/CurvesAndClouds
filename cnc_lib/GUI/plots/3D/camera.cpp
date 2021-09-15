#include "camera.h"
#ifdef CNC_OPENGL
#include <iostream>

cnc::graphics::Camera::Camera()
{
    pos = vec3(0,0,2);
    center = vec3(0,0,0);
    up = vec3(0,1,0);

    fov = 45.0f;
    aspect_ratio = 1;
    zNear = 0.01;
    zFar = 100;

    computeView();
}

void cnc::graphics::Camera::change_aspect(int w, int h)
{
    aspect_ratio = float(w) / float(h ? h : 1);
    computeView();
}

void cnc::graphics::Camera::computeView()
{
    View.setToIdentity();
    View.lookAt(pos,center,up);
    mat4 proj;
    proj.setToIdentity();
    proj.perspective(fov,aspect_ratio,zNear,zFar);
    View = proj*View;
}

#endif
