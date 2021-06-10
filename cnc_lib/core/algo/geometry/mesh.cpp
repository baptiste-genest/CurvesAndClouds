#include "mesh.h"

cnc::algo::geometry::Mesh::Mesh(const std::vector<Triangle> &T)
{
    h_edges.resize(T.size()*3);
    uint count =0;
    for (const auto& t : T){
        faces.push_back(t);
        for (uint k = 0;k<3;k++){
            h_edges[count+k] = new halfedge;
        }
        for (uint k = 0;k<3;k++){
            h_edges[count+k]->vertex = t.m_vertex[k];
            h_edges[count+k]->next = h_edges[count+(k+1)%3];
            h_edges[count+k]->face_id = count/3;
        }
        count += 3;
    }



}

cnc::algo::geometry::Mesh::~Mesh()
{
    for (const auto& h : h_edges)
        delete h;
}
