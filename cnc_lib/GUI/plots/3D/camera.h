#ifndef CAMERA_H
#define CAMERA_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef CNC_OPENGL

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

    inline vec3 getCamPos() const {
        return pos;
    }

    inline void setCamPos(const vec3& x) {
        pos = x;
        computeView();
    }
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

#endif

#endif // CAMERA_H
