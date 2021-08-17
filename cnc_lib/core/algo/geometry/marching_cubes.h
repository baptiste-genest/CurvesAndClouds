#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif
#include "core/algo/calculus/calculus.h"
#include "cnc_types.h"
#include "marching_table.h"
#include "core/algo/bit_operations.h"

#include <QVector3D>

namespace cnc {

namespace algo {

namespace geometry {

class MarchingCubes
{
public:
    MarchingCubes(const calculus::scalar_function& f,scalar iso_level,uint N,range X);
    MarchingCubes(const calculus::scalar_function& f,uint N,range X,range Y,range Z);

private:
    uint N;
    std::vector<scalar> samples;
    inline uint index(uint i,uint j,uint k) const {
        return k*N*N + j*N + i;
    }

    void march_cell(uint x,uint y,uint z);

    scalar isolevel;
    std::vector<uint> faces;
    std::vector<int> edge_to_vertex_id;
    std::vector<QVector3D> vertices;
    std::map<std::pair<uint,uint>,uint> edge_to_vertex;
};

}

}

}
#endif // MARCHING_CUBES_H
