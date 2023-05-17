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

    void handleStart(const edge &e);
    void handleSplit(const edge &e);
    void handleEnd(const edge &e);
    void handleMerge(const edge &e);
    void handleRegular(const edge &e_pr, const edge &e);

    std::vector<edge> edges;
    std::vector<vertex> vertices;
    std::set<edge> T;
};

std::ostream &operator<<(std::ostream &os, polygon &poly)
{
    for (auto& edge : poly.edges) os << edge << std::endl; 
    return os;
}

#include "polygon.cpp"

#endif