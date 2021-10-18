#include "skiplist.h"

SkipList::SkipList(float m_p): p(m_p)
{
    if (p < 1e-4){
        max_height = 1;
        p = 0.f;
    }
    else
        max_height = int(std::log(0.05)/std::log(p))+1;
    sentry = new SkipCell(max_height,0);
}

SkipList::~SkipList()
{
    delete sentry;
}

void SkipList::insert(Element e)
{
    int h = get_random_height();
    highest = std::max(highest,h-1);
    auto first = get_first();
    if (first == nullptr){
        sentry->insert_in_front(e,h,sentry->cells);
    }
    else if (e < first->info){
        sentry->insert_in_front(e,h,sentry->cells);
    }
    else {
        uint steps = 0;
        Cell* ptr = sentry->cells[highest];
        std::vector<Cell*> path;
        //std::cout << "INSERT " << e << std::endl;
        while (ptr != nullptr){
            //std::cout << ptr->height << ' ' << ptr->parent->info << std::endl;
            steps++;
            if (ptr->next != nullptr){
                if (ptr->next->parent->info < e){
                    ptr = ptr->next;
                    continue;
                }
                if (ptr->next->parent->info == e)
                    return;
            }
            if (ptr->height <= h)
                path.push_back(ptr);
            ptr = SkipCell::get_under_cell(ptr);
        }
        //std::cout << "NB STEPS " << steps << std::endl;
        path.back()->parent->insert_in_front(e,h,path);
    }
}

void SkipList::print() const
{
    if (sentry->next == nullptr)
        puts("Empty SkipList");
    else
        sentry->next->print();
}

bool SkipList::is_sorted() const
{
    auto ptr = sentry->next;
    while (ptr != nullptr){
        if (ptr->next == nullptr)
            return true;
        if (ptr->info > ptr->next->info)
            return false;
        ptr = ptr->next;
    }
    return true;
}

bool SkipList::belong(Element e) const
{
    Cell* ptr = sentry->cells.back();
    std::vector<Cell*> path;
    while (ptr != nullptr){
        if (ptr->next != nullptr){
            if (ptr->next->parent->info < e){
                ptr = ptr->next;
                continue;
            }
            if (ptr->next->parent->info == e)
                return true;
        }
        ptr = SkipCell::get_under_cell(ptr);
    }
    return false;
}

int SkipList::get_random_height()
{
    int h = 1;
    while (binomial(p) && h < max_height)
        h++;
    return h;
}

