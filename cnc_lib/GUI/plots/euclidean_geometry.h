#ifndef EUCLIDEAN_GEOMETRY_H
#define EUCLIDEAN_GEOMETRY_H

#include <QPainter>
#include "cnc_types.h"
#include "cnc_error.h"

namespace cnc {

class Primitive {
private:
    friend class EuclideanGeometry;
    virtual void compute_values() = 0;
    virtual void draw(QPainter& Qp) = 0;
};

class EuclideanGeometry
{
public:
    EuclideanGeometry();
};

}
#endif // EUCLIDEAN_GEOMETRY_H
