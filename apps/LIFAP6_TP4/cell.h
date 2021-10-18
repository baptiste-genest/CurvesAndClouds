#ifndef CELL_H
#define CELL_H

using Element = int;

class SkipCell;

struct Cell
{
    Cell(SkipCell* p,int h) {
        parent = p;
        height = h;
    }
    Cell* next = nullptr;
    SkipCell* parent = nullptr;
    int height;
};

#endif // CELL_H
