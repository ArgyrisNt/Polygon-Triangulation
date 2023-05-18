#ifndef H_POLYGON
#define H_POLYGON

#include <set>
#include <vector>
#include "utilities.h"

class polygon
{
public:
    polygon(const std::vector<edge>& _edges) : edges(_edges) {}
    ~polygon() {}

    void makeMonotone();

    friend std::ostream &operator<<(std::ostream &os, polygon &poly);

private:
    void constructVertices();
    void identifyVertex(vertex &v);

    void handleStart(edge &e);
    void handleSplit(edge &e);
    void handleEnd(edge &e);
    void handleMerge(edge &e);
    void handleRegular(edge &e_pr, edge &e);

    std::vector<edge> edges;
    std::vector<vertex*> vertices;
    std::set<edge> T;
};

std::ostream &operator<<(std::ostream &os, polygon &poly)
{
    for (auto& edge : poly.edges) os << edge << std::endl; 
    return os;
}

#include "polygon.cpp"

#endif