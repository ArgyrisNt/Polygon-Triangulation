#ifndef H_EDGE
#define H_EDGE

#include "vertex.h"

class face;

class edge
{
public:
    edge() : start(nullptr), previous(nullptr), next(nullptr), helper(nullptr), f(nullptr), id(-1) {}
    edge(edge* _previous, vertex* _start, edge* _next, int _id = -1) 
    {previous = _previous; start = _start; next = _next; helper = nullptr; f = nullptr; id = _id;}

    friend std::ostream &operator<<(std::ostream &os, const edge &e);

    edge *previous;
    edge *next;
    vertex *start;
    vertex *helper;
    face *f;
    int id;
};

std::ostream &operator<<(std::ostream &os, const edge &e)
{
    os << *e.start << " , " << *e.next->start << " ";
    return os;
}


#endif