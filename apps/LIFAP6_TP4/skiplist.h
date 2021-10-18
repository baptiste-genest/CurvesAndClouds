#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "scell.h"
#include "stdlib.h"
#include "math.h"

class SkipList
{
public:
    SkipList(float p = 0.5f);
    ~SkipList();
    void insert(Element e);
    void print() const;
    bool is_sorted() const;
    bool belong(Element e) const;

private:
    int max_height;
    int highest = 0;
    float p;
    int get_random_height();

    SkipCell* get_first() {
        return sentry->next;
    }
    SkipCell* sentry = nullptr;
};

#endif // SKIPLIST_H
