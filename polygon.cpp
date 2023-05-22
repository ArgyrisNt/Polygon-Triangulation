#include "polygon.h"
#include <iostream>
#include <algorithm>

#define PI 3.1415926535897932384

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
}



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

void polygon::addEdge(edge* _previous, vertex* _start, edge* _next)
{
    edge newEdge1 = edge(_previous, _start, _next, edges.size());
    edge newEdge2 = edge(_next->previous, _next->start, _previous->next, edges.size() + 1);
    edges.push_back(newEdge1);
    edges.push_back(newEdge2);

    edges[edges.size() - 1].next->previous = &edges[edges.size() - 1];
    edges[edges.size() - 1].previous->next = &edges[edges.size() - 1];
    edges[edges.size() - 2].previous->next = &edges[edges.size() - 2];
    edges[edges.size() - 2].next->previous = &edges[edges.size() - 2];

    int N = faces.size();
    face newFace1(faces[faces.size() - 1].id + 1);
    face newFace2(faces[faces.size() - 1].id + 2);
    newFace1.e = &edges[edges.size() - 2];
    newFace2.e = &edges[edges.size() - 1];
    for (auto iter = faces.begin(); iter != faces.end(); ++iter)
    {
        if (_previous->f->id == iter->id)
        {
            faces.erase(iter);
            break;
        }
    }
    faces.push_back(newFace1);
    faces.push_back(newFace2);

    edge *current = &edges[edges.size() - 1];
    do
    {
        current->f = &faces[faces.size() - 1];
        current = current->next;
    }
    while (current->start != edges[edges.size() - 1].start);

    current = &edges[edges.size() - 2];
    do
    {
        current->f = &faces[faces.size() - 2];
        current = current->next;
    } 
    while (current->start != edges[edges.size() - 2].start);
}



void polygon::handleStart(edge* e)
{
    e->helper = e->start;
    T.insert(e);
}

void polygon::handleSplit(edge *e)
{
    auto ej = T.begin();
    //addEdge(e->previous, e->start, (*ej)->helper->e);
    addEdge((*ej)->helper->e->previous, (*ej)->helper, e);
    edges[(*ej)->id].helper = e->start;
    e->helper = e->start;
    T.insert(e);
}

void polygon::handleEnd(edge *e_pr)
{
    if (e_pr->helper->type == 3)
        addEdge(e_pr, e_pr->next->start, e_pr->helper->e);
    T.erase(e_pr);
}

void polygon::handleMerge(edge *e_pr)
{
    if (e_pr->helper->type == 3)
        addEdge(e_pr, e_pr->next->start, e_pr->helper->e);
    T.erase(e_pr);
    auto ej = T.begin();
    if ((*ej)->helper->type == 3)
        addEdge(e_pr, e_pr->next->start, (*ej)->helper->e);
    edges[(*ej)->id].helper = e_pr->next->start;
}

void polygon::handleRegular(edge *e_pr, edge *e)
{
    if (*e->next->start < *e->start)
    {
        if (e_pr->helper->type == 3)
            addEdge(e_pr, e->start, e_pr->helper->e);
        T.erase(e_pr);
        e->helper = e->start;
        T.insert(e);
    }
    else
    {
        auto ej = T.begin();
        if ((*ej)->helper->type == 3)
            addEdge(e_pr, e->start, (*ej)->helper->e);
        edges[(*ej)->id].helper = e->start;
    }
}
