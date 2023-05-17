// Partition a polygon into monotone pieces

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <set>
#include <fstream>

#define PI 3.1415926535897932384

bool almostEqual(double x, double y)
{
    return (fabs(x - y) < 1e-10);
}

struct vertex
{
    vertex() {}
    vertex(double _x, double _y) : x(_x), y(_y) {}
    double x, y;
    int id;
    int type;
    vertex *previous;
    vertex *next;

    bool operator<(const vertex& v) const
    {
        if (almostEqual(this->y, v.y)) return this->x > v.x;
        return this->y < v.y;
    }
};

inline std::ostream& operator<<(std::ostream& os, const vertex& v)
{
    os << "(" << v.x << ", " << v.y << ")";
}

struct edge
{
    vertex start, end;
    mutable vertex helper;

    inline bool operator<(const edge& e) const { return this->start < e.start; }
};

inline std::ostream &operator<<(std::ostream &os, const edge &e)
{
    os << "[" << e.start << ", " << e.end << "]";

    return os;
}

int identifyVertex(vertex& v)
{
    vertex vu(v.previous->x - v.x, v.previous->y - v.y);
    vertex vw(v.next->x - v.x, v.next->y - v.y);
    double theta = atan2(vu.y, vu.x) - atan2(vw.y, vw.x);
    if (theta < 0) { theta += 2 * PI; }
    if (*v.previous < v && *v.next < v)
    {
        if (theta < PI) return 0; // start vertex
        else return 1; // split vertex
    }
    else if (v < *v.previous && v < *v.next)
    {
        if (theta < PI) return 2; // end vertex
        else return 3; // merge vertex
    }
    else return 4; // regular vertex
}

void handleStart(edge &e, std::set<edge> &T)
{
    e.helper = e.start;
    T.insert(e);
}

void handleSplit(edge &e, std::set<edge>& T, std::vector<edge>& edges)
{
    auto ej = T.begin();
    edges.push_back({e.start, (*ej).helper});
    (*ej).helper = e.start;
    e.helper = e.start;
    T.insert(e);
}

void handleEnd(edge &e, std::set<edge> &T, std::vector<edge> &edges)
{
    if (e.helper.type == 3)
        edges.push_back({e.end, e.helper});
    T.erase(e);
}

void handleMerge(edge &e, std::set<edge> &T, std::vector<edge> &edges)
{
    if (e.helper.type == 3)
        edges.push_back({e.end, e.helper});
    T.erase(e);
    auto ej = T.begin();
    if ((*ej).helper.type == 3)
        edges.push_back({e.end, (*ej).helper});
    (*ej).helper = e.end;
}

void handleRegular(edge &e_pr, edge &e, std::set<edge> &T, std::vector<edge> &edges)
{
    if (e.end < e.start)
    {
        if (e_pr.helper.type == 3)
            edges.push_back({e.start, e_pr.helper});
        T.erase(e_pr);
        e.helper = e.start;
        T.insert(e);
    }
    else
    {
        auto ej = T.begin();
        if ((*ej).helper.type == 3)
            edges.push_back({e.start, (*ej).helper});
        (*ej).helper = e.start;
    }
}

void makeMonotone(std::vector<edge>& edges)
{
    std::vector<vertex> vertices(edges.size());
    for (int i = 0; i < edges.size(); ++i)
    {
        vertices[i] = edges[i].start;
        vertices[i].previous = (i == 0) ? &edges[edges.size() - 1].start : &edges[i - 1].start;
        vertices[i].next = &edges[i].end;
        vertices[i].id = i;
    }

    std::sort(vertices.begin(), vertices.end(), [](vertex& v1, vertex& v2) {return v2 < v1;});

    std::set<edge> T;
    for (int i = 0; i < vertices.size(); ++i)
    {
        vertices[i].type = identifyVertex(vertices[i]);
        int j = (vertices[i].id == 0) ? vertices.size() - 1 : vertices[i].id - 1;
        if (vertices[i].type == 0) 
        {
            handleStart(edges[vertices[i].id], T); 
        }
        else if (vertices[i].type == 1)
        {
            handleSplit(edges[vertices[i].id], T, edges);
        }
        else if (vertices[i].type == 2)
        {
            handleEnd(edges[j], T, edges);
        }
        else if (vertices[i].type == 3)
        {
            handleMerge(edges[j], T, edges);
        }
        else if (vertices[i].type == 4)
        {
            handleRegular(edges[j], edges[vertices[i].id], T, edges);
        }
    }
}

int main()
{
    std::vector<edge> edges{{{0.0, 0.0}, {1.0, -1.0}}, {{1.0, -1.0}, {2.0, 1.0}}, {{2.0, 1.0}, {3.0, 0.0}}, {{3.0, 0.0}, {4.0, 3.0}}, {{4.0, 3.0}, {2.0, 4.0}},
                            {{2.0, 4.0}, {1.0, 3.0}}, {{1.0, 3.0}, {0.72, 1.58}}, {{0.72, 1.58}, {0.24, 1.84}}, {{0.24, 1.84}, {0.0, 0.0}}};
    makeMonotone(edges);

    return 0;
}