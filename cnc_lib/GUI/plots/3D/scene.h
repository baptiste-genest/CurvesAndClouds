#ifndef SCENE_H
#define SCENE_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include <QMatrix4x4>
#include "object.h"
#include "GLWrapper.h"

namespace cnc {

namespace graphics {

class Scene
{
public:
    Scene();

    template <typename PrimitiveType,typename ... Args>
    Object& add_object(Args&& ... args) {
        PrimitiveType* P = new PrimitiveType(args...);
        m_objects.push_back(Object(P));
        return m_objects.back();
    }

    void init();
    void resize(int w,int h);
    void draw();

protected:
    mat4 m_view;
    std::vector<Object> m_objects;
};

}

}

#endif // SCENE_H
