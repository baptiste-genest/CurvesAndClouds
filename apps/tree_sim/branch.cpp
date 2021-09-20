#include "branch.h"

Branch::Branch()
{
    energy_cost = 0;
    energy_gain = 0;
    length = MIN_LENGTH;
    width = MIN_WIDTH;
}

scalar Branch::collectOutEnergy()
{
    scalar S = computeVolume()*GRAVITY;
    int offset = (directSon != nullptr) ? 1 : 0;
    if (offset){
        scalar x = directSon->collectOutEnergy();
        branches_out_energy[0] = x;
        S += x;
    }
    for (uint k = 0;k<splits.size();k++){
        scalar x = splits[k]->collectOutEnergy();
        branches_out_energy[k+offset] = x;
        S += x;
    }
    energy_cost = S;
    return S;
}

scalar Branch::collectInEnergy(const QVector3D &sol_pos,scalar time)
{
    scalar C = 0;
    int offset = (directSon != nullptr) ? 1 : 0;
    if (offset){
        scalar x = directSon->collectInEnergy(sol_pos,time);
        branches_in_energy[0] = x;
        C += x;
    }
    for (uint k = 0;k<splits.size();k++){
        scalar x = splits[k]->collectInEnergy(sol_pos,time);
        branches_in_energy[k+offset] = x;
        C += x;
    }
    energy_gain = C;
    return C;
}

void Branch::growth(scalar in_energy)
{
    scalar energy_for_self = in_energy*branch_self_prioritizing;
    scalar remaining_energy = in_energy - energy_for_self;
    int offset = (directSon != nullptr) ? 1 : 0;
    if (offset)
        directSon->growth(remaining_energy);
    else {
        std::vector<scalar> w = computeRedistributiveWeights();
        int offset = (directSon != nullptr) ? 1 : 0;
        for (uint k = 0;k<splits.size();k++)
            splits[k+offset]->growth(in_energy*w[k+offset]);
    }
    scalar length_growth_factor = length_growth_kernel();
    if (length < MAX_LENGTH)
        length += remaining_energy*length_growth_factor;
    scalar width_growth_factor = (1-length_growth_factor)*width_growth_kernel();
    if (width < MAX_WIDTH)
        width += remaining_energy*width_growth_factor;
    strength += remaining_energy*(1-width_growth_factor);

    if (length >= MAX_LENGTH && directSon == nullptr)
        directSon = new Branch(this,pos + dir*length,(dir+rand_vec3()*.1).normalized());
}

Branch::Branch(Branch *parent, const QVector3D &_pos, const QVector3D &_dir)
    :	Branch()
{
    father = parent;
    pos = _pos;
    dir = _dir;
}

std::vector<scalar> Branch::computeRedistributiveWeights() const
{
    std::vector<scalar> w((directSon != nullptr) + splits.size());

    int offset = (directSon != nullptr) ? 1 : 0;
    if (offset)
        w[0] = branches_in_energy[0] - branches_out_energy[0];
    for (uint k = 0;k<splits.size();k++)
        w[k+offset] = branches_in_energy[k] - branches_out_energy[k];

    scalar sum_delta = energy_gain - energy_cost;
    for (scalar& x : w){
        x /= sum_delta;
        x = std::pow(x,POWER_SPLITTING);
    }
    scalar new_sum = 0;
    for (const scalar& x : w)
        new_sum += x;
    for (scalar& x : w)
        x /= new_sum;
    return w;
}

scalar Branch::getCumulatedHeight() const
{
    if (father == nullptr)
        return 0;
    return length + father->getCumulatedHeight();
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
