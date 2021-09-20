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

scalar Leaf::computeLeafArea() const
{
    return radius*radius*M_PI;
}
