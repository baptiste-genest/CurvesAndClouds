#ifndef DIAGRAMPLOTTER_H
#define DIAGRAMPLOTTER_H

#include "../../../core/algo/geometry/diagrams/diagrams.h"
#include "../plottable.h"

namespace cnc {

class DiagramPlotter
{
public:
    DiagramPlotter(const algo::geometry::Diagram& D);

private:
    const algo::geometry::Diagram& D;
};

}

#endif // DIAGRAMPLOTTER_H
