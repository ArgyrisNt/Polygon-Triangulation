#ifndef H_POLYGON
#define H_POLYGON

#include <set>
#include <vector>
#include <deque>
#include "edge.h"
#include "vertex.h"

class polygon
{
public:
    polygon(std::vector<vertex>& _vertices) : vertices(_vertices) { constructEdges(); }
    ~polygon() {}

    void makeMonotone();
    std::vector<vertex> &getVertices() { return vertices; }
    std::deque<edge> &getEdges() { return edges; }

    friend std::ostream &operator<<(std::ostream &os, const polygon &poly);

private:
    void constructEdges();
    void identifyVertex(vertex &v);
    void addEdge(edge* _previous, vertex* _start, edge* _next);

    void handleStart(edge *e);
    void handleSplit(edge *e);
    void handleEnd(edge *e_previous);
    void handleMerge(edge *e_previous);
    void handleRegular(edge *e_previous, edge *e);

    std::vector<vertex> vertices;
    std::deque<edge> edges;
    std::set<edge*> T;
};

std::ostream &operator<<(std::ostream &os, const polygon &poly)
{
    for (auto& edge : poly.edges) os << edge << std::endl;
    return os;
}

#include "polygon.cpp"

#endif