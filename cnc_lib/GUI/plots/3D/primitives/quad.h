#ifndef QUAD_H
#define QUAD_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include "primitive.h"
#include "../GLWrapper.h"

namespace cnc {

namespace graphics {

class Quad : public Primitive
{
public:
    Quad(const std::vector<vec3>& points);

    virtual void onDraw(const mat4& view) override;
    virtual void init() override;

private:
    std::vector<index> m_index;



};


}

}

#endif // QUAD_H
