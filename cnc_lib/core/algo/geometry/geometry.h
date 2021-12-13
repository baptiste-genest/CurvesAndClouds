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
    GeometricContext(const cloud& C);

    vec get_vec_edge(const topo::edge& e);
    vec face_direction(const topo::edge& E,topo::vertex O);
    scalar facet_angle(const topo::face& F,topo::vertex O);

private:
    cloud points;
};

//QVector3D line_line_intersection(const QVector3D& o1,const QVector3D& d1,const QVector3D& o2,const QVector3D& d2);

}

}

}


#endif // GEOMETRY_H
