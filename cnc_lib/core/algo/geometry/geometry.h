#ifndef GEOMETRY_H
#define GEOMETRY_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "cnc_error.h"
#include "../lin_alg.h"
#include <QVector3D>
#include <memory>
#include "combinatorial_complexes.h"

namespace cnc {

namespace algo {

namespace geometry {

class Mesh2;

struct mesh_generation;

struct edge{
    vec* a;
    vec* b;
};

vec get_plane_dual(const vec& N);

scalar isotropy_score(const std::array<vec,3>& v);
scalar isotropy_score(const vec& a,const vec& b,const vec& c);

vec cross(const vec& a,const vec& b);
scalar det22(const vec& a,const vec& b);

vec rayRayIntersection(const vec& O1,const vec& D1,const vec& O2,const vec& D2,bool& intersects);
vec segmentSegmentIntersection(const vec& A1,const vec& A2,const vec& B1,const vec& B2,bool& intersects);
vec lineLineIntersection(const vec& O1,const vec& D1,const vec& O2,const vec& D2);
vec argLineLineIntersection(const vec& O1,const vec& D1,const vec& O2,const vec& D2);

vec get2DOutwardNormal(const vec& A,const vec& B,const vec& C);

using circum_data = std::pair<vec,scalar>;

class GeometricContext {
public:
    GeometricContext() {}
    GeometricContext(const cloud& C);
    GeometricContext(cloud&& C);

    vec get_vec_edge(const topology::edge& e) const;
    vec get_vec_edge(const topology::edge& e,topology::vertex x) const;
    vec face_direction(const topology::edge& E,topology::vertex O) const;
    scalar facet_angle(const topology::face& F,const topology::edge& E,topology::vertex O) const;
    topology::vertices getVertices() const;
    topology::indexed_vertices getIndexedVertices() const;

    topology::vertex add_vertex(const vec& x);
    //vec get2DOutwardNormal(const topology::face& F,const topology::edge& E)const;

    vec operator()(const topology::vertex& v) const;
    vec midPoint(const topology::face& F) const;
    vec midPoint(const topology::edge& E) const;
    const vec& getPoint(topology::vertex x) const;

    const cloud& getPoints() const;
    cloud& getPoints();

    scalar PolygonArea(const topology::SimplicialPolygon& P) const;
    scalar ConvexPolygonArea(const topology::SimplicialPolygon& P) const;
    scalar PolygonSignedArea(const topology::SimplicialPolygon& P) const;
    vec compute2DPolygonCentroid(const topology::SimplicialPolygon& P) const;
    void uniformizeOrientation(topology::SimplicialPolygon& P) const;

    scalar circumRadius(const topology::face& F) const ;
    vec circumCenter(const topology::face& F) const ;
    circum_data circumCenterRadius(const topology::face& F) const;
private:
    friend struct mesh_generation;
    cloud points;
};

using GeometricContextRef = std::shared_ptr<GeometricContext>;


using line = std::pair<vec,vec>;
vec smallest_positive_ray_square_intersection(const vec& O,const vec& D,scalar R);

vec get_intersection(const line& A,const line& B);

//QVector3D line_line_intersection(const QVector3D& o1,const QVector3D& d1,const QVector3D& o2,const QVector3D& d2);

}}}


#endif // GEOMETRY_H
