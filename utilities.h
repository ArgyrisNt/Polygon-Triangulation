#ifndef H_UTILITIES
#define H_UTILITIES

#include <math.h>
#include <fstream>
#include <memory>

struct vertex
{
    vertex() {}
    vertex(double _x, double _y) : x(_x), y(_y) {}

    bool operator<(const vertex& v) const
    {
        if (fabs(this->y - v.y) < 1e-10) return this->x > v.x;
        return this->y < v.y;
    }

    vertex& operator=(const vertex& v)
    {
        x = v.x;
        y = v.y;
        id = v.id;
        type = v.type;
        previous = v.previous;
        next = v.next;
        return *this;
    }

    double x, y;
    int id, type;
    vertex* previous;
    vertex* next;
};

struct edge
{
    inline bool operator<(const edge &e) const 
    { return this->start < e.start; }

    edge& operator=(const edge& e)
    {
        start = e.start;
        end = e.end;
        helper = e.helper;
        id = e.id;
        return *this;
    }

    vertex start, end;
    mutable vertex helper;
    int id;
};

inline std::ostream& operator<<(std::ostream& os, const vertex& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const edge &e)
{
    os << "[" << e.start << ", " << e.end << "]";
    return os;
}


#endif