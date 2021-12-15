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
#include "combinatorial_complexes.h"

namespace cnc {

namespace algo {

namespace geometry {

struct edge{
    vec* a;
    vec* b;
};

scalar isotropy_score(const std::array<vec,3>& v);
scalar isotropy_score(const vec& a,const vec& b,const vec& c);

vec cross(const vec& a,const vec& b);

class GeometricContext {
public:
    GeometricContext(const cloud& C);

    vec get_vec_edge(const topology::edge& e) const;
    vec face_direction(const topology::edge& E,topology::vertex O) const;
    scalar facet_angle(const topology::face& F,const topology::edge& E,topology::vertex O) const;
    topology::vertices getVertices() const;
    topology::indexed_vertices getIndexedVertices() const;

    vec operator()(const topology::vertex& v) const;

    const cloud& getPoints() const;
    cloud& getPoints();

private:
    cloud points;
};

//QVector3D line_line_intersection(const QVector3D& o1,const QVector3D& d1,const QVector3D& o2,const QVector3D& d2);

}

}

}


#endif // GEOMETRY_H
