#include "tube_primitive.h"


cnc::graphics::Tube::Tube(const std::vector<cnc::graphics::vec3> &nodes,const std::vector<float>& radiuses, uint circular_resolution, uint height_resolution)
    :	m_nodes(nodes),
      m_c_rez(circular_resolution),
      m_h_rez(height_resolution)
{
    m_vertices.clear();
    m_indexArray.clear();
    const static QVector3D x(1,0.456,.123);
    QVector3D ring[circular_resolution];
    for (uint k = 0;k<circular_resolution;k++){
        scalar th = 2*M_PI*k/(circular_resolution-1);
        ring[k] = QVector3D(cos(th),sin(th),0);
    }
    //std::vector<QVector3D> rings(nodes.size());
    for (uint k = 0;k<nodes.size()-1;k++){
        QMatrix4x4 B;
        std::array<QVector3D,3> b;
        b[0] = (nodes[k+1] - nodes[k]).normalized();
        b[1] = QVector3D::crossProduct(b[0],x).normalized();
        b[2] = QVector3D::crossProduct(b[0],b[1]).normalized();
        for (uint j = 0;j<3;j++)
            for (uint i = 0;i<3;i++)
                B(j,i) = b[2-j][i];
        for (const auto& r : ring)
            m_vertices.push_back(B*r*radiuses[k] + nodes[k]);
    }
    for (uint k = 0;k<nodes.size()-1;k++){
        for (uint i = 0;i<circular_resolution;i++){
            uint i0 =(k)*circular_resolution+i;
            uint i1 =(k)*circular_resolution+((i+1)%circular_resolution);
            uint i2 =(k+1)*circular_resolution;
            uint i3 =(k+1)*circular_resolution+((i+1)%circular_resolution);
            m_indexArray.push_back(i0);
            m_indexArray.push_back(i1);
            m_indexArray.push_back(i2);
            m_indexArray.push_back(i3);
        }
    }
    computeNormals();
}

cnc::graphics::Tube::Tube(const std::vector<cnc::graphics::vec3> &nodes, cnc::scalar radius, uint circular_resolution, uint height_resolution)
    : Tube(nodes,std::vector<float>(nodes.size(),radius),circular_resolution,height_resolution)
{}
