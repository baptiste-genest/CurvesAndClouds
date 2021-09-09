#ifndef SCENE_VIEWER_H
#define SCENE_VIEWER_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_error.h"
#include "scene.h"
#include <QOpenGLWidget>

namespace cnc {

class SceneViewer : public QOpenGLWidget {
public:
    SceneViewer(QWidget* parent = nullptr);
    ~SceneViewer();

    inline void initializeGL();
    inline void resizeGL(int w, int h);
    inline void paintGL();

    inline graphics::Scene* getScene(){
        return m_scene;
    }


private:
    graphics::Scene* m_scene;

    void initShaders();

    const std::string vertex_shader_loc = "/home/eulerson/Curves-Clouds/cnc_lib/GUI/plots/3D/vshaderGL3.glsl";
    const std::string fragment_shader_loc = "/home/eulerson/Curves-Clouds/cnc_lib/GUI/plots/3D/fshaderGL3.glsl";

    int m_LightPosLoc = 0;

};

}

#endif // SCENE_VIEWER_H
