#ifndef H_POLYGON
#define H_POLYGON

#include <set>
#include <vector>
#include <deque>
#include <map>
#include <stack>
#include "face.h"

class polygon
{
public:
    polygon(std::vector<vertex>& _vertices) : vertices(_vertices) { constructEdges(); }
    ~polygon() {}

    void makeMonotone();
    void triangulate();

    std::vector<vertex> &getVertices() { return vertices; }
    std::deque<edge> &getEdges() { return edges; }
    std::deque<face> &getFaces() { return faces; }

    friend std::ostream &operator<<(std::ostream &os, const polygon &poly);

private:
    void constructEdges();
    void identifyVertex(vertex &v);
    void addEdge(edge* _previous, vertex* _start, edge* _next);
    void updateEdges(edge& e1, edge& e2);
    void eraseOldFace(face* f);

    void handleStart(edge *e);
    void handleSplit(edge *e);
    void handleEnd(edge *e_previous);
    void handleMerge(edge *e_previous);
    void handleRegular(edge *e_previous, edge *e);

    std::vector<vertex *> computeMonotonePiece(edge *e);
    std::map<vertex *, int> findChainOfVertices(std::vector<vertex *> &monotonePiece);
    bool diagonalIsInside(vertex *v1, vertex *v2, vertex *v3);
    void handleVerticesOnDifferentChain(vertex *v_pr, vertex *v, std::stack<vertex *> &S);
    void handleVerticesOnSameChain(vertex *v, std::stack<vertex *> &S);
    void handleBottomMostVertex(vertex *v, std::stack<vertex *> &S);

    std::vector<vertex> vertices;
    std::deque<edge> edges;
    std::deque<face> faces;
    std::set<edge*> T;

    bool isMonotone;
};

std::ostream &operator<<(std::ostream &os, const polygon &poly)
{
    for (auto &f : poly.faces)
    {
        std::cout << f << "\n\n";
    }
    return os;
}

#include "polygon.cpp"

#endif