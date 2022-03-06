#include "sdf.h"

cnc::scalar cnc::algo::geometry::SDF::toSegment(const cnc::vec &p, const cnc::vec &a, const cnc::vec &b)
{
    vec pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), (scalar)0.0,(scalar) 1.0 );
    return (pa - ba*h).norm();
}
