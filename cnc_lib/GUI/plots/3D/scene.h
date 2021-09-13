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
#include "lights/light_source.h"
#include "camera.h"

#include <utility>

namespace cnc {

namespace graphics {

class Scene
{
public:
    Scene();
    ~Scene();

    template <typename PrimitiveType,typename ... Args>
    std::pair<Object*,PrimitiveType*> add_object(Args&& ... args) {
        PrimitiveType* P = new PrimitiveType(std::forward<Args>(args)...);
        m_objects.push_back(new Object(P));
        return {m_objects.back(),P};
    }

    Camera* getCam() {
        return cam;
    }

    void init();
    void resize(int w,int h);
    void draw();

private:
    std::vector<Object*> m_objects;

    Camera* cam = nullptr;
    std::vector<LightSource*> sources;
};

}

}

#endif // SCENE_H
