#include "branch.h"

Branch::Branch()
{

}

scalar Branch::collectOutEnergy()
{
    scalar S = computeVolume();
    if (directSon != nullptr)
        S += directSon->computeVolume();
    for (TreeElement* s : splits)
        S += s->computeVolume();
    energy_cost = S;
    return S;
}

scalar Branch::collectInEnergy(const QVector3D &sol_pos,scalar time)
{
    scalar C = 0;
    if (directSon != nullptr){
        scalar x = directSon->collectInEnergy(sol_pos,time);
        branches_energy[0] = x;
        C += x;
    }
    for (uint k = 0;k<splits.size();k++){
        scalar x = splits[k]->collectInEnergy(sol_pos,time);
        branches_energy[k+1] = x;
        C += x;
    }
    energy_gain = C;
    return C;
}

void Branch::growth(scalar in_energy)
{

}

scalar Branch::computeVolume() const
{
    scalar w2;
    if (isSonLeafOrVoid())
        w2 = MIN_WIDTH;
    else
        w2 = ((Branch*)directSon)->width;
    return (width*width + w2*w2 + width*w2)*M_PI*length/3;
}
