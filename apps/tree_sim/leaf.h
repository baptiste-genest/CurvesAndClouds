#ifndef LEAF_H
#define LEAF_H

#include "tree_element.h"

class Leaf : public TreeElement
{
public:
    Leaf();
    inline bool isLeaf() const override {
        return true;
    }
    virtual inline scalar getWidth() const override {
        return MIN_WIDTH;
    }
    virtual inline scalar computeVolume() const override {
        return 0;
    }

    virtual inline scalar collectOutEnergy() override;

    virtual scalar collectInEnergy(const QVector3D& sol_pos,scalar time) override;

    virtual void growth(scalar in_energy) override;

private:
    scalar radius;
    scalar collected_energy;
    QVector3D leaf_normal;
    QVector3D pos;
    inline scalar computeLeafArea() const;
};

#endif // LEAF_H
