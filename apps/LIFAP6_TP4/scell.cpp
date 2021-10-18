#include "scell.h"

SkipCell::SkipCell(int h, Element e)
{
    info = e;
    for (int k = 0;k<h;k++)
        add_cell();
    height = h;
}

SkipCell::~SkipCell()
{
    for (auto c : cells)
        delete c;
    if (next != nullptr)
        delete next;
}


Cell *SkipCell::get_under_cell(Cell *c)
{
    if (c->height == 0)
        return nullptr;
    return c->parent->cells[c->height-1];
}

void SkipCell::print()
{
    std::cout << '(' << info << ',' << height << ") ";
    if (next == nullptr)
        std::cout << std::endl;
    else
        next->print();
}

SkipCell* SkipCell::insert_in_front(Element e, int h,const std::vector<Cell*>& path)
{
    auto old_next = next;
    next = new SkipCell(h,e);
    next->next = old_next;

    //puts("PATH");
    for (auto c : path){
        if (c->height < h){
            auto tmp = c->next;
            c->next = next->cells[c->height];
            next->cells[c->height]->next = tmp;
        }
    }
    return next;
}

Cell *SkipCell::add_cell(){
    int h = cells.size();
    Cell* c = new Cell(this,h);
    cells.push_back(c);
    height++;
    return c;
}

bool binomial(float p)
{
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r < p;
}
