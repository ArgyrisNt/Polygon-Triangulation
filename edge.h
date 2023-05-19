#ifndef H_EDGE
#define H_EDGE

#include <fstream>
#include "vertex.h"

class edge
{
public:
    edge() : start(nullptr), previous(nullptr), next(nullptr), helper(nullptr) {}

    void operator=(const edge &e)
    {
        start = e.start;
        previous = e.previous;
        next = e.next;
    }

    friend std::ostream &operator<<(std::ostream &os, const edge &e);

    vertex *start;
    edge *previous;
    edge *next;
    vertex *helper;
    int id;
};

std::ostream &operator<<(std::ostream &os, const edge &e)
{
    os << *e.start << " , " << *e.next->start << " ";
    return os;
}


#endif