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

    scalar computeVolume() const override;
    scalar collectOutEnergy() override;
    virtual scalar collectInEnergy(const QVector3D& sol_pos,scalar time) override;
    void growth(scalar in_energy) override;

private:
    Branch(Branch* parent,const QVector3D& _pos,const QVector3D& _dir);

    QVector3D dir;
    QVector3D pos;
    scalar length;
    scalar width;
    std::vector<TreeElement*> splits;
    std::vector<scalar> branches_energy;
    scalar energy_cost;
    scalar energy_gain;
    TreeElement* directSon = nullptr;
    Branch* father = nullptr;

    constexpr static int POWER_SPLITTING = 2;
    void computeRedistributiveWeights(std::vector<scalar>& w) const;

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

    scalar collectInEnergy() const;

    scalar branch_self_prioritizing = 0.2;

};

#endif // BRANCH_H
