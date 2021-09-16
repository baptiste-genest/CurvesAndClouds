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

    virtual void init(const Material& M) override;
    virtual void onDraw(const Material& M) override;

    /*
    inline void add_vertex(const vec3& x);
    inline void add_face(index,index,index);
    */

private:

    loc m_normalBufferId;
    loc m_normalLoc;

    void loadBuffers() override;

    std::vector<vec3> m_vertices;
    std::vector<vec3> m_normals;
    std::vector<index> m_indexArray;

    //std::vector<index> m_narray;
};

}

}

#endif
#endif // MESH_H
