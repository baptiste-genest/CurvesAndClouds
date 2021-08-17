#include "mesh.h"

cnc::algo::geometry::Mesh::Mesh(const std::vector<Triangle> &T)
{
/*
    h_edges.resize(T.size()*3);
    faces.resize(T.size());
    uint count =0,tid;
    for (const auto& t : T){
        tid = count/3;
        faces[tid] = t;
        for (uint k = 0;k<3;k++){
            h_edges[count+k] = new halfedge;
        }
        for (uint k = 0;k<3;k++){
            h_edges[count+k]->vertex = t.m_vertex[k];
            h_edges[count+k]->next = h_edges[count+(k+1)%3];
            h_edges[count+k]->face_id = tid;
        }
        count += 3;
    }
*/
}

cnc::algo::geometry::Mesh::~Mesh()
{
    for (const auto& h : h_edges)
        delete h;
}
