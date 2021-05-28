#include "mesh.h"

cnc::vec cnc::algo::geometry::vertex_ref::operator()() const
{
    return container->vertex[id];
}

cnc::algo::geometry::vertex_ref cnc::algo::geometry::Mesh::add_vertex(const cnc::vec &x)
{
    vertex.push_back(x);
    return vertex_ref(vertex.size()-1,this);
}

void cnc::algo::geometry::Mesh::add_face(const cnc::algo::geometry::Triangle &t)
{
    faces.push_back(t);
}
