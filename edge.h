#ifndef H_EDGE
#define H_EDGE

#include <fstream>
#include "vertex.h"

class edge
{
public:
    edge() : start(nullptr), previous(nullptr), next(nullptr), helper(nullptr), id(-1) {}
    edge(edge* _previous, vertex* _start, edge* _next) 
    {previous = _previous; start = _start; next = _next; helper = nullptr; id = -1;}

    friend std::ostream &operator<<(std::ostream &os, const edge &e);

    edge *previous;
    edge *next;
    vertex *start;
    vertex *helper;
    int id;
};

std::ostream &operator<<(std::ostream &os, const edge &e)
{
    os << *e.start << " , " << *e.next->start << " ";
    return os;
}


#endif