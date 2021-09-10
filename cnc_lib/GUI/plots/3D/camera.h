#ifndef CAMERA_H
#define CAMERA_H

#include "graphics.h"

namespace cnc {

namespace graphics {

class Camera
{
public:
    Camera();

    inline mat4 getViewMatrix() const {
        return View;
    }

    inline void change_aspect(int w,int h);
private:
    void computeView();

    mat4 View;

    vec3 pos;
    vec3 center;
    vec3 up;
    float fov;
    float zNear;
    float zFar;
    float aspect_ratio;
};


}

}

#endif // CAMERA_H
