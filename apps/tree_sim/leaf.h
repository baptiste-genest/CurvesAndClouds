#ifndef LEAF_H
#define LEAF_H

#include "tree_element.h"
#include "branch.h"

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
    inline QVector3D getPosition() const override { return father->getPosition(); }
    inline scalar getAltitude() const override { return father->getAltitude(); }
    virtual void log(uint offset = 0) const override;
    virtual void buildMesh(graphics::Scene* S) override {}
    virtual void directSonChain(std::vector<QVector3D>& p,std::vector<scalar>& r,graphics::Scene* S) override {}

private:
    scalar radius;
    scalar collected_energy;
    QVector3D leaf_normal;
    QVector3D pos;
    Branch* father;
    inline scalar computeLeafArea() const;
};

#endif // LEAF_H
