#ifndef H_FACE
#define H_FACE

#include "edge.h"

class face
{
public:
    face() : e(nullptr), id(-1) {}
    face(int _id, edge* _e = nullptr) : id(_id) , e(_e) {}

    friend std::ostream &operator<<(std::ostream &os, const face &f);

    edge* e;
    int id;
};

std::ostream &operator<<(std::ostream &os, const face &f)
{
    os << "Polygon " << f.id << " is:\n";
    edge* current = f.e;
    do
    {
        os << *current << "  ";
        current = current->next;
    }
    while (current->start != f.e->start);
    return os;
}

#endif