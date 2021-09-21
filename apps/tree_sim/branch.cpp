#include "branch.h"

Branch::Branch()
{
    energy_cost = 0;
    energy_gain = 0;
    length = MIN_LENGTH;
    width = MIN_WIDTH;
    dir = QVector3D(0,0,1);
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
    if (father == nullptr)
        C = SOIL_ENERGY;
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
    scalar energy_for_self, remaining_energy;
    if (hasNoSon()){
        //remaining_energy = 0;
        energy_for_self = in_energy;
    }
    else {
        energy_for_self = in_energy*branch_self_prioritizing;
        remaining_energy = in_energy - energy_for_self;
        int offset = (directSon != nullptr) ? 1 : 0;
        if (offset) {
            directSon->growth(remaining_energy);
        }
        else {
            std::vector<scalar> w = computeRedistributiveWeights();
            int offset = (directSon != nullptr) ? 1 : 0;
            for (uint k = 0;k<splits.size();k++)
                splits[k+offset]->growth(remaining_energy*w[k+offset]);
        }
    }
    scalar length_growth_factor = length_growth_kernel();
    if (length < MAX_LENGTH)
        length += energy_for_self*length_growth_factor;
    scalar width_growth_factor = (1-length_growth_factor)*width_growth_kernel();
    if (width < MAX_WIDTH)
        width += energy_for_self*width_growth_factor;
    strength += energy_for_self*(1-width_growth_factor);

    if (length >= MAX_LENGTH*0.5 && directSon == nullptr)
        addSon();
    scalar split_prob = 0.05*(getAltitude()/MAX_HEIGHT);
    if (energy_for_self > 1e-1){
        if (cnc::algo::stat::random_var::random_scalar(0,1) < split_prob)
            addSplit();
    }
}

void Branch::performLifeCycle(const QVector3D &sol_pos, scalar time)
{
    collectInEnergy(sol_pos,time);
    collectOutEnergy();
    scalar DE = std::max(0.f,energy_gain - energy_cost);
    growth(DE);
}

Branch::Branch(Branch *parent,scalar dff, const QVector3D &_dir)
    :	Branch()
{
    father = parent;
    dist_from_father_root = dff;
    dir = _dir;
}

void Branch::addSon()
{
    directSon = new Branch(this,1.f,(dir+rand_vec3()*.1).normalized());
    branches_in_energy.push_back(0);
    branches_out_energy.push_back(0);
}

void Branch::addSplit()
{
    splits.push_back(new Branch(this,length/MAX_LENGTH,(dir+rand_vec3()*.9).normalized()));
    branches_in_energy.push_back(0);
    branches_out_energy.push_back(0);
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
    if (sum_delta < 1e-5){
        for (scalar& x : w)
            x = 1./w.size();
    }
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

QVector3D Branch::getPosition() const
{
    if (father == nullptr)
        return QVector3D();
    return father->getPosition() + father->dir*father->length*dist_from_father_root;
}

scalar Branch::getAltitude() const
{
    if (father == nullptr)
        return 0;
    return father->getAltitude() + father->length*dist_from_father_root;
}

void Branch::log(uint offset) const
{
    std::string off;
    for (uint k = 0;k<offset;k++)
        off += "	";
    if (father == nullptr)
        std::cout << off <<  "ROOT:" << std::endl;
    else
        std::cout << off << "BRANCH:" << std::endl;
    std::cout << off << "WIDTH : " << width << std::endl;
    std::cout << off << "LENGTH : " << length << std::endl << std::endl;
    if (directSon != nullptr)
        directSon->log(offset+1);
    for (auto s : splits)
        s->log(offset+1);
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
