#ifndef PRIMITIVE_MESH_H
#define PRIMITIVE_MESH_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "primitive.h"

namespace cnc {

namespace graphics {

class Mesh : public Primitive
{
public:
    Mesh();

    virtual void init(const Material& M) override;
    virtual void onDraw(const QMatrix4x4 &mvp) override;

private:
    std::vector<vec3> m_vertices;
    std::vector<index> m_varray;

    std::vector<vec3> m_normals;
    std::vector<index> m_narray;
};

}

}

#endif // MESH_H
