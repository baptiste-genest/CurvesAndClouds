#ifndef TUBE_PRIMITIVE_H
#define TUBE_PRIMITIVE_H

#include "mesh_primitive.h"
#include <QMatrix3x3>
#include <QQuaternion>


namespace cnc {

namespace graphics {

class Tube : public Mesh
{
public:
    Tube(const std::vector<vec3>& nodes,const std::vector<float>& radiuses,uint circular_resolution,uint height_resolution);
    inline Tube(const std::vector<vec3>& nodes,scalar radius,uint circular_resolution,uint height_resolution);
protected:
    std::vector<vec3> m_nodes;
    uint m_c_rez;
    uint m_h_rez;

};

}

}

#endif // TUBE_PRIMITIVE_H
