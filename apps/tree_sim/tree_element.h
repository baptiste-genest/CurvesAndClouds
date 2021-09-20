#ifndef TREE_ELEMENT_H
#define TREE_ELEMENT_H

#include <curvesandcloud.h>
using namespace cnc;

class Branch;

inline QVector3D rand_vec3(){
    auto x = algo::stat::random_var::sample_vector_on_unit_sphere(3,1)[0];
    return QVector3D(x(0),x(1),x(2));
}

class TreeElement
{
public:
    virtual bool isLeaf() const = 0;
    virtual scalar getWidth() const = 0;
    virtual scalar computeVolume() const = 0;
    virtual scalar collectOutEnergy() = 0;
    virtual scalar collectInEnergy(const QVector3D& sol_pos,scalar time)= 0;
    virtual void growth(scalar in_energy) = 0;
protected:
    static scalar growth_kernel(scalar x);

    constexpr static scalar MIN_WIDTH = 1.;
    constexpr static scalar MAX_WIDTH = 30.;
    constexpr static scalar MIN_LENGTH = 3.;
    constexpr static scalar MAX_LENGTH = 40.;
    constexpr static scalar MIN_RADIUS = 1.;
    constexpr static scalar MAX_RADIUS = 20.;
    constexpr static scalar SOIL_ENERGY = 30.;
    constexpr static scalar GRAVITY = 1e-3;

    Branch* father = nullptr;
};

#endif // TREE_ELEMENT_H
