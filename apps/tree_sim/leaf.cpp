#include "leaf.h"

scalar Leaf::collectInEnergy(const QVector3D &sol_pos, scalar time)
{
    QVector3D I = (sol_pos-pos).normalized();
    return radius*radius*M_PI*std::max(0.f,-QVector3D::dotProduct(I,leaf_normal))*time;
}

void Leaf::growth(scalar in_energy)
{

}
