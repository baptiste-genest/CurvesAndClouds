#ifndef PARAMETRIZATION_H
#define PARAMETRIZATION_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "core/algo/geometry/geometry.h"
#include "core/algo/geometry/simpleglmesh.h"
#include "core/algo/linear_algebra/lin_alg.h"
#include <QVector2D>

namespace cnc {

namespace algo {

namespace geometry {

class LSCMParam{
public:
    LSCMParam(SimpleGLMesh* m,uint fixed_vertex_0,const QVector2D& p1,uint fixed_vertex_1,const QVector2D& p2);

    scalar current_ELSCM_value() const;
private:
    void get_v_value(uint f,vec& v) const;
    void get_u_value(uint f,vec& v) const;

    std::pair<QVector3D,QVector3D> get_triangle_orthobasis(uint f);
    void compute_gradient_matrix_per_triangle();
    scalar get_gradient_component(uint i);

    uint nb_vert,nb_faces;
    SimpleGLMesh* mesh;
    std::vector<scalar> u_coord;
    std::vector<scalar> v_coord;
    std::vector<mat> M;
    uint fixed1;
    uint fixed2;
    const scalar dx = 1e-2;
    const mat ortho = mat(2,2,{0,-1,1,0});
};

}

}

}

#endif // PARAMETRIZATION_H
