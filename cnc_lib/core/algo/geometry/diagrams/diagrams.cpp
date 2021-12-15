#include "diagrams.h"

const cnc::algo::topology::SimplicialPolygon &cnc::algo::geometry::Diagram::operator[](int i) const
{
    return cells[i];
}
