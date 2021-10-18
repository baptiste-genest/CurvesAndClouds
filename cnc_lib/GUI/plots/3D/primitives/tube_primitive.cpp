#include "tube_primitive.h"


cnc::graphics::Tube::Tube(const std::vector<cnc::graphics::vec3> &nodes,const std::vector<float>& radiuses, uint circular_resolution, uint height_resolution)
    :	m_nodes(nodes),
      m_c_rez(circular_resolution),
      m_h_rez(height_resolution)
{
    m_vertices.clear();
    m_indexArray.clear();
    const static vec3 x(0.321,0.534,.551);

    const float th = 360./circular_resolution;
    vec3 d,d_p;
    for (uint k = 0;k<nodes.size();k++){
        if ( k < nodes.size()-1){
            d = (nodes[k+1]-nodes[k]).normalized();
            d_p = QVector3D::crossProduct(d,x);
        }
        for (uint i = 0;i<circular_resolution;i++){
            QQuaternion R = QQuaternion::fromAxisAndAngle(d,th*i);
            auto y = R*d_p;
            m_vertices.push_back(nodes[k] + y.normalized()*radiuses[k]);
        }
    }
    uint rez = circular_resolution;
    for (uint k = 0;k<nodes.size()-1;k++){
        for (uint i = 0;i<circular_resolution;i++){
            uint i0 =(k)*rez+i;
            uint i1 =(k)*rez+((i+1)%circular_resolution);
            uint i2 =(k+1)*rez+i;
            uint i3 =(k+1)*rez+((i+1)%circular_resolution);
            m_indexArray.push_back(i0);
            m_indexArray.push_back(i2);
            m_indexArray.push_back(i1);
            m_indexArray.push_back(i1);
            m_indexArray.push_back(i2);
            m_indexArray.push_back(i3);
        }
    }
    Mesh::TRIANGLE_COUNT = m_indexArray.size()/3;
    Mesh::computeNormals();
}

cnc::graphics::Tube::Tube(const std::vector<cnc::graphics::vec3> &nodes, cnc::scalar radius, uint circular_resolution, uint height_resolution)
    : Tube(nodes,std::vector<float>(nodes.size(),radius),circular_resolution,height_resolution)
{}
