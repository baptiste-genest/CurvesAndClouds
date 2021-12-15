#ifndef MESH2_H
#define MESH2_H

#include "geometry.h"
#include "combinatorial_complexes.h"

namespace cnc {

namespace algo {

namespace geometry {

using namespace topology;

using NormalMap = std::map<topology::face,topology::faceComp,vec>;

class Mesh2
{
public:
    Mesh2(GeometricContext& C,const topology::faces& Fa,const EdgeFaceConnectivityGraph& EF,const NormalMap& NM);

private:
    GeometricContext& G;
    topology::vertices V;
    topology::edges E;
    topology::faces F;
    topology::EdgeFaceConnectivityGraph EtoF;
    NormalMap N;
};

}}}

#endif // MESH2_H
