#ifndef QUAD_H
#define QUAD_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef CNC_OPENGL

#include "primitive.h"
#include "../GLWrapper.h"

namespace cnc {

namespace graphics {

class Quad : public Primitive
{
public:
    Quad(const std::vector<vec3>& pos);
    Quad(const std::string& loc,float scale);

    virtual void init(const Material& M) override;
    virtual void onDraw(const Material& M) override;

private:

    loc m_normalBufferId;
    loc m_normalLoc;

    void loadBuffers() override;

    std::vector<vec3> m_vertices;
    std::vector<vec3> m_normals;
    std::vector<index> m_indexArray;
};


}

}

#endif

#endif // QUAD_H
