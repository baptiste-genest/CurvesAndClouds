#ifndef PRIMITIVE_MESH_H
#define PRIMITIVE_MESH_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#ifdef CNC_OPENGL

#include "primitive.h"
#include "core/algo/inout.h"

namespace cnc {

namespace graphics {

class Mesh : public Primitive
{
public:
    Mesh(const std::vector<vec3>& pos,const std::vector<vec3>& normals,const std::vector<index> faces);
    Mesh(const std::string& loc,float scale);
    Mesh() {}

    virtual void init(const Material& M) override;
    virtual void onDraw(const Material& M) override;

protected:

    void computeNormals();
    scalar computeFaceArea(index f) const;
    vec3 computeFaceNormal(index f) const;


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
#endif // MESH_H
