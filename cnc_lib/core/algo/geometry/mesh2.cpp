#include "mesh2.h"

cnc::algo::geometry::Mesh2::Mesh2(GeometricContext &C, const faces &Fa, const EdgeFaceConnectivityGraph &EF, const NormalMap &NM) : G(C),F(Fa),EtoF(EF),N(NM)
{
    for (const auto& f : F)
        for (const auto& e : f){
            E.insert(e);
            for (const auto& v : e)
                V.insert(v);
        }

}
