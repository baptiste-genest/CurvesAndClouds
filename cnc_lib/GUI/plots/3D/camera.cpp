#include "camera.h"

cnc::graphics::Camera::Camera()
{
    pos = vec3(0,0,5);
    center = vec3(0,0,0);
    up = vec3(0,1,0);

    fov = 45;
    aspect_ratio = 1;
    zNear = -1.0;
    zFar = 1.0;

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
    View.perspective(fov,aspect_ratio,zNear,zFar);
}
