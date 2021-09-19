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

    virtual inline scalar collectOutEnergy() override {
        return 0;
    }

    virtual scalar collectInEnergy(const QVector3D& sol_pos,scalar time) override;

    virtual void growth(scalar in_energy) override;

private:
    scalar radius;
    QVector3D leaf_normal;
    QVector3D pos;
};

#endif // LEAF_H
