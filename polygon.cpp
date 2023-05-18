#include "polygon.h"
#include <iostream>
#include <algorithm>

#define PI 3.1415926535897932384

void polygon::makeMonotone()
{
    constructVertices();
    std::sort(vertices.begin(), vertices.end(), [](vertex* v1, vertex* v2) {return *v2 < *v1;});

    for (int i = 0; i < vertices.size(); ++i)
    {
        if (vertices[i]->type == 0) {handleStart(edges[vertices[i]->id]);} 
        else if (vertices[i]->type == 1) {handleSplit(edges[vertices[i]->id]);}
        else if (vertices[i]->type == 2) {handleEnd(edges[vertices[i]->previous->id]);}
        else if (vertices[i]->type == 3) 
        {
            handleMerge(edges[vertices[i]->previous->id]);
        }
        else if (vertices[i]->type == 4)
        {
            handleRegular(edges[vertices[i]->previous->id], edges[vertices[i]->id]);
        }
    }
}



void polygon::constructVertices()
{
    vertices.clear();
    for (int i = 0; i < edges.size(); ++i)
    {
        edges[i].id = i;
        edges[i].start.previous = (i == 0) ? &edges[edges.size() - 1].start : &edges[i - 1].start;
        edges[i].start.next = &edges[i].end;
        edges[i].start.id = i;
        identifyVertex(edges[i].start);
        vertices.push_back(&edges[i].start);
        (i == 0) ? edges[edges.size() - 1].end = edges[0].start : edges[i - 1].end = edges[i].start; 
    }
}

void polygon::identifyVertex(vertex &v)
{
    vertex vu(v.previous->x - v.x, v.previous->y - v.y);
    vertex vw(v.next->x - v.x, v.next->y - v.y);
    double theta = atan2(vu.y, vu.x) - atan2(vw.y, vw.x);
    if (theta < 0) { theta += 2 * PI; }
    if (*v.previous < v && *v.next < v)
    {
        if (theta < PI) v.type = 0; // start vertex
        else v.type = 1; // split vertex
    }
    else if (v < *v.previous && v < *v.next)
    {
        if (theta < PI) v.type = 2; // end vertex
        else v.type = 3; // merge vertex
    }
    else v.type = 4; // regular vertex
}



void polygon::handleStart(edge &e)
{
    e.helper = e.start;
    T.insert(e);
}

void polygon::handleSplit(edge &e)
{
    auto ej = T.begin();
    edges.push_back({e.start, ej->helper});
    ej->helper = e.start;
    edges[ej->id].helper = e.start;
    e.helper = e.start;
    T.insert(e);
}

void polygon::handleEnd(edge &e)
{
    if (e.helper.type == 3)
        edges.push_back({e.end, e.helper});
    T.erase(e);
}

void polygon::handleMerge(edge &e)
{
    if (e.helper.type == 3)
        edges.push_back({e.end, e.helper});
    T.erase(e);
    auto ej = T.begin();
    if (ej->helper.type == 3)
        edges.push_back({e.end, ej->helper});
    ej->helper = e.end;
    edges[ej->id].helper = e.end;
}

void polygon::handleRegular(edge &e_pr, edge &e)
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
        if (ej->helper.type == 3)
            edges.push_back({e.start, ej->helper});
        ej->helper = e.start;
        edges[ej->id].helper = e.start;
    }
}
