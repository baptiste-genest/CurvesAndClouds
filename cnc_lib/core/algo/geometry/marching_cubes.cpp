#include "marching_cubes.h"

cnc::algo::geometry::MarchingCubes::MarchingCubes(const cnc::algo::calculus::scalar_function &f, scalar iso_level, uint n, cnc::range X) : N(n),isolevel(iso_level)
{
    auto nodes = calculus::get_lin_space(X,(int)N);
    samples.resize(N*N*N);
    edge_to_vertex_id.resize(N*N*N,-1);
    scalar x,y,z;
    for (uint ix = 0;ix<N;ix++){
        x = nodes[ix];
        for (uint iy = 0;iy<N;iy++){
            y = nodes[iy];
            for (uint iz = 0;iz<N;iz++){
                z = nodes[iz];
                samples[index(ix,iy,iz)] = f(vec({x,y,z}));
            }
        }
    }
}

void cnc::algo::geometry::MarchingCubes::march_cell(uint x, uint y, uint z)
{
    using namespace MARCHING_CUBE_TABLES;
    uint cube_index = 0,id;
    for (uint k = 0;k<8;k++){
        id = index(x + get_bit(k,0),y+ get_bit(k,1),z+ get_bit(k,2));
        if (samples[id] < isolevel)
            cube_index |= (1 << k);
    }
    uint cut_edges = edges[cube_index];
    if (cut_edges == 0)
        return;
}
