#include <algorithm>
#include "polygon.h"

#define PI 3.1415926535897932384

void polygon::constructEdges()
{
    edges = std::deque<edge>(vertices.size());
    faces = std::deque<face>(1);
    for (int i = 0; i < vertices.size(); ++i)
    {
        edges[i].id = i;
        edges[i].start = &vertices[i];
        edges[i].previous = (i == 0) ? &edges[edges.size() - 1] : &edges[i - 1];
        edges[i].next = (i == vertices.size() - 1) ? &edges[0] : &edges[i + 1];
        edges[i].f = &faces[0];

        vertices[i].e = &edges[i];
    }
    faces[0].e = &edges[0];
    faces[0].id = 0;

    for (auto& v : vertices) identifyVertex(v);
    isMonotone = false;
}

void polygon::identifyVertex(vertex &v)
{
    vertex vu(*v.e->previous->start - v);
    vertex vw(*v.e->next->start - v);
    double theta = atan2(vu.y, vu.x) - atan2(vw.y, vw.x);
    if (theta < 0) { theta += 2 * PI; }
    if (*v.e->previous->start < v && *v.e->next->start < v)
    {
        if (theta < PI) v.type = 0; // start vertex
        else v.type = 1; // split vertex
    }
    else if (v < *v.e->previous->start && v < *v.e->next->start)
    {
        if (theta < PI) v.type = 2; // end vertex
        else v.type = 3; // merge vertex
    }
    else v.type = 4; // regular vertex
}



void polygon::makeMonotone()
{
    std::vector<vertex> sortedVertices(vertices);
    std::sort(sortedVertices.begin(), sortedVertices.end(), [](const vertex& v1, const vertex& v2) {return v2 < v1;});

    for (int i = 0; i < sortedVertices.size(); ++i)
    {
        if (sortedVertices[i].type == 0) {handleStart(sortedVertices[i].e);} 
        else if (sortedVertices[i].type == 1) {handleSplit(sortedVertices[i].e);}
        else if (sortedVertices[i].type == 2) {handleEnd(sortedVertices[i].e->previous);}
        else if (sortedVertices[i].type == 3) {handleMerge(sortedVertices[i].e->previous);}
        else if (sortedVertices[i].type == 4) {handleRegular(sortedVertices[i].e->previous, sortedVertices[i].e);}
    }
    isMonotone = true;
}

void polygon::handleStart(edge* e)
{
    e->helper = e->start;
    T.insert(e);
}

void polygon::handleSplit(edge *e)
{
    auto ej = T.begin();
    addEdge((*ej)->helper->e->previous, (*ej)->helper, e);
    edges[(*ej)->id].helper = e->start;
    e->helper = e->start;
    T.insert(e);
}

void polygon::handleEnd(edge *e_pr)
{
    if (e_pr->helper->type == 3) addEdge(e_pr, e_pr->next->start, e_pr->helper->e);
    T.erase(e_pr);
}

void polygon::handleMerge(edge *e_pr)
{
    if (e_pr->helper->type == 3) addEdge(e_pr, e_pr->next->start, e_pr->helper->e);
    T.erase(e_pr);
    auto ej = T.begin();
    if ((*ej)->helper->type == 3) addEdge(e_pr, e_pr->next->start, (*ej)->helper->e);
    edges[(*ej)->id].helper = e_pr->next->start;
}

void polygon::handleRegular(edge *e_pr, edge *e)
{
    bool isOnLeftBoundary = (*e->next->start < *e->start);
    if (isOnLeftBoundary)
    {
        if (e_pr->helper->type == 3) addEdge(e_pr, e->start, e_pr->helper->e);
        T.erase(e_pr);
        e->helper = e->start;
        T.insert(e);
    }
    else
    {
        auto ej = T.begin();
        if ((*ej)->helper->type == 3) addEdge(e_pr, e->start, (*ej)->helper->e);
        edges[(*ej)->id].helper = e->start;
    }
}

void polygon::addEdge(edge* _previous, vertex* _start, edge* _next)
{
    edges.push_back(edge(_previous, _start, _next, edges.size()));
    edges.push_back(edge(_next->previous, _next->start, _previous->next, edges.size() + 1));

    int Nfaces = faces.size();
    eraseOldFace(_previous->f);
    faces.push_back(face(faces[Nfaces - 1].id + 1, &*(edges.end() - 2)));
    faces.push_back(face(faces[Nfaces - 1].id + 2, &edges.back()));

    updateEdges(edges.back(), *(edges.end() - 2));
}

void polygon::updateEdges(edge &e1, edge &e2)
{
    e1.next->previous = &e1;
    e1.previous->next = &e1;
    e2.previous->next = &e2;
    e2.next->previous = &e2;

    edge *current = &e1;
    do
    {
        current->f = &faces.back();
        current = current->next;
    } while (current->start != e1.start);

    current = &e2;
    do
    {
        current->f = &*(faces.end() - 2);
        current = current->next;
    } while (current->start != e2.start);
}

void polygon::eraseOldFace(face* f)
{
    for (auto iter = faces.begin(); iter != faces.end(); ++iter)
    {
        if (f->id == iter->id)
        {
            faces.erase(iter);
            break;
        }
    }
}



void polygon::triangulate()
{
    if (!isMonotone) return;

    for (int i = 0; i < faces.size(); ++i)
    {
        std::vector<vertex *> monotonePiece = computeMonotonePiece(faces[i].e);
        std::sort(monotonePiece.begin(), monotonePiece.end(), [](vertex* v1, vertex* v2) {return *v2 < *v1;});
        std::map<vertex*, int> chain = findChainOfVertices(monotonePiece); // 1st-> id of edge , 2nd -> 0:left, 1:right

        std::stack<vertex*> S;
        S.push(monotonePiece[0]);
        S.push(monotonePiece[1]);
        for (int j = 2; j < monotonePiece.size() - 1; ++j)
        {
            if (chain[monotonePiece[j]] != chain[S.top()]) handleVerticesOnDifferentChain(monotonePiece[j - 1], monotonePiece[j], S);
            else handleVerticesOnSameChain(monotonePiece[j], S);
        }
        handleBottomMostVertex(monotonePiece[monotonePiece.size() - 1], S);
    }
}

std::vector<vertex*> polygon::computeMonotonePiece(edge* e)
{
    std::vector<vertex*> monotonePiece;
    edge* current = e;
    do
    {
        monotonePiece.push_back(current->start);
        current = current->next;
    }
    while (current->start != e->start);

    return monotonePiece;
}

std::map<vertex*, int> polygon::findChainOfVertices(std::vector<vertex*>& monotonePiece)
{
    std::map<vertex*, int> chain;
    edge *current = monotonePiece[0]->e;
    chain[current->start] = 0;
    do
    {
        edge *after = current->next;
        if (*after->start < *current->start) chain[current->start] = 0;
        else chain[current->start] = 1;
        current = after;
    } while (current->start != monotonePiece[0]->e->start);

    return chain;
}

void polygon::handleVerticesOnDifferentChain(vertex* v_pr, vertex* v, std::stack<vertex*>& S)
{
    while (!S.empty())
    {
        if (S.size() > 1) edges.push_back(edge(v->e->previous, v, S.top()->e));
        S.pop();
    }
    S.push(v_pr);
    S.push(v);
}

void polygon::handleVerticesOnSameChain(vertex* v, std::stack<vertex*>& S)
{
    vertex *last = S.top();
    S.pop();
    while (diagonalIsInside(v, S.top(), last))
    {
        edges.push_back(edge(v->e->previous, v, S.top()->e));
        last = S.top();
        S.pop();
        if (S.empty()) break;
    }
    S.push(last);
    S.push(v);
}

void polygon::handleBottomMostVertex(vertex *v, std::stack<vertex *> &S)
{
    while (S.size() > 1)
    {
        S.pop();
        edges.push_back(edge(v->e->previous, v, S.top()->e));
    }
    S.pop();
}

bool polygon::diagonalIsInside(vertex* v1, vertex* v2, vertex* v3)
{
    vertex v1v3(*v3 - *v1);
    vertex v1v2(*v2 - *v1);

    int exterior = v1v2.x * v1v3.y - v1v3.x * v1v2.y;
    return (exterior > 0) ? true : false;
}
