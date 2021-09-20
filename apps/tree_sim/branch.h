#ifndef BRANCH_H
#define BRANCH_H

#include "tree_element.h"

class Branch : public TreeElement
{
public:
    Branch();
    inline bool isLeaf() const override {
        return false;
    }

    virtual inline scalar getWidth() const override {
        return width;
    }

    virtual scalar computeVolume() const override;
    virtual scalar collectOutEnergy() override;
    virtual scalar collectInEnergy(const QVector3D& sol_pos,scalar time) override;
    void growth(scalar in_energy) override;

    void performLifeCycle(const QVector3D& sol_pos,scalar time);

    QVector3D getPosition() const override;
    scalar getAltitude() const override;

    virtual void log(uint offset = 0) const override;

private:
    Branch(Branch* parent,scalar dff,const QVector3D& _dir);

    QVector3D dir;
    QVector3D pos;
    scalar length;
    scalar width;
    scalar strength;
    std::vector<TreeElement*> splits;
    std::vector<scalar> splits_lengths_from_root;
    scalar dist_from_father_root = 0;

    std::vector<scalar> branches_in_energy;
    std::vector<scalar> branches_out_energy;
    scalar energy_cost;
    scalar energy_gain;
    TreeElement* directSon = nullptr;


    constexpr static int POWER_SPLITTING = 2;
    std::vector<scalar> computeRedistributiveWeights() const;

    inline bool isSonLeaf() const
    {
        if (directSon == nullptr)
            return false;
        return directSon->isLeaf();
    }
    inline bool isSonLeafOrVoid() const
    {
        if (directSon == nullptr)
            return true;
        return directSon->isLeaf();
    }
    inline bool isRoot() const
    {
        return father == nullptr;
    }

    inline bool hasNoSon() const
    {
        return directSon == nullptr && !splits.size();
    }

    scalar collectInEnergy() const;

    inline scalar width_growth_kernel() const {
        scalar y = width/MAX_WIDTH;
        return std::exp(-y)*(1-y);
    }

    inline scalar length_growth_kernel() const {
        scalar y = length/MAX_WIDTH;
        return std::exp(-y)*(1-y);
    }


    scalar branch_self_prioritizing = 0.4;

};

#endif // BRANCH_H
