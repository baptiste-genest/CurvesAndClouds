#include "tube_primitive.h"


cnc::graphics::Tube::Tube(const std::vector<cnc::graphics::vec3> &nodes,const std::vector<float>& radiuses, uint circular_resolution, uint height_resolution)
    :	m_nodes(nodes),
      m_c_rez(circular_resolution),
      m_h_rez(height_resolution)
{

}

cnc::graphics::Tube::Tube(const std::vector<cnc::graphics::vec3> &nodes, cnc::scalar radius, uint circular_resolution, uint height_resolution)
    : Tube(nodes,std::vector<float>(nodes.size(),radius),circular_resolution,height_resolution)
{}
