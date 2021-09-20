#include "leaf.h"

scalar Leaf::collectOutEnergy() {
    return computeLeafArea()*GRAVITY;
}

scalar Leaf::collectInEnergy(const QVector3D &sol_pos, scalar time)
{
    QVector3D I = (sol_pos-pos).normalized();
    collected_energy = computeLeafArea()*std::max(0.f,-QVector3D::dotProduct(I,leaf_normal))*time;
    return collected_energy;
}

void Leaf::growth(scalar in_energy)
{

}

void Leaf::log(uint offset) const
{
    std::string off;
    for (uint k = 0;k<offset;k++)
        off += "	";
    std::cout << off << "LEAF:" << std::endl;
    std::cout << off << "radius:" << radius << std::endl;
}



scalar Leaf::computeLeafArea() const
{
    return radius*radius*M_PI;
}
