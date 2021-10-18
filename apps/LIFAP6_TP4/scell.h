#ifndef SCELL_H
#define SCELL_H

#include "cell.h"
#include <iostream>
#include <vector>

bool binomial(float p);

using rit = std::vector<Cell*>::reverse_iterator;
class SkipCell
{
public:
    SkipCell(int h,Element e);
    ~SkipCell();
private:
    friend class SkipList;
    static Cell* get_under_cell(Cell* c);

    void print();
    SkipCell* insert_in_front(Element e,int height,const std::vector<Cell*>& path);

    Cell* add_cell();

    Element info;
    std::vector<Cell*> cells;
    SkipCell* next = nullptr;
    int height = 0;
};

#endif // SCELL_H
