#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef CNC_OPENGL
#include <QMatrix4x4>
#include <GL/gl.h>
#include "../color.h"
#include "../graphics.h"
#include "../GLWrapper.h"
#include "../material.h"

namespace cnc {

namespace graphics {

class Primitive {
public:
    Primitive();
    virtual ~Primitive() {}

    virtual void init(const Material& M) = 0;
    virtual void onDraw(const mat4& view);

protected:
    virtual void getUniformsLoc();
    virtual void getAttributesLoc();

    void loadBuffers();
    void loadUniforms(const mat4& view);

    std::vector<loc> m_attributesLoc;

    //shaders adress
    shader_id m_shaderId;
    GLint m_glVertexLoc;
    GLint m_glColorLoc;

    // Buffers
    loc m_VertexBufferId;
    loc m_IndexBufferId;
    loc m_ColorBufferId;

    loc m_viewMatrixLoc;
    loc m_localMatrixLoc;

    mat4 local_transform;

    std::vector<vec3> m_points;
    std::vector<vec3> m_normals;
    std::vector<col> m_colors;

    std::string vertex_shader_loc;
    std::string frag_shader_loc;

    int TRIANGLE_COUNT = 0;

    bool initialized = false;

};


}

}

#endif

#endif // PRIMITIVE_H
