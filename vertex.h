#ifndef H_VERTEX
#define H_VERTEX

#include <math.h>
#include <fstream>
#include "edge.h"

class edge;

class vertex
{
public:
    vertex(double _x, double _y) : x(_x), y(_y), e(nullptr), type(-1) {}

    void operator=(const vertex &v)
    {
        x = v.x;
        y = v.y;
        e = v.e;
    }

    bool operator<(const vertex& v) const
    {
        if (fabs(this->y - v.y) < 1e-10) return this->x > v.x;
        return this->y < v.y;
    }

    friend std::ostream &operator<<(std::ostream &os, const vertex &v);

    double x, y;
    edge *e;
    int type;
};

std::ostream &operator<<(std::ostream &os, const vertex &v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

#endif