#include "polygon.h"
#include <iostream>
#include <algorithm>

#define PI 3.1415926535897932384

void polygon::makeMonotone()
{
    constructVertices();
    std::sort(vertices.begin(), vertices.end(), [](vertex& v1, vertex& v2) {return v2 < v1;});

    for (int i = 0; i < vertices.size(); ++i)
    {
        identifyVertex(vertices[i]);
        int j = (vertices[i].id == 0) ? vertices.size() - 1 : vertices[i].id - 1;
        if (vertices[i].type == 0) {handleStart(edges[vertices[i].id]);} 
        else if (vertices[i].type == 1) {handleSplit(edges[vertices[i].id]);}
        else if (vertices[i].type == 2) {handleEnd(edges[j]);}
        else if (vertices[i].type == 3) {handleMerge(edges[j]);}
        else if (vertices[i].type == 4) {handleRegular(edges[j], edges[vertices[i].id]);}
    }
}



void polygon::constructVertices()
{
    vertices.clear();
    for (int i = 0; i < edges.size(); ++i)
    {
        vertex temporary(edges[i].start);
        temporary.previous = (i == 0) ? &edges[edges.size() - 1].start : &edges[i - 1].start;
        temporary.next = &edges[i].end;
        temporary.id = i;
        vertices.push_back(temporary);
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



void polygon::handleStart(const edge &e)
{
    e.helper = e.start;
    T.insert(e);
}

void polygon::handleSplit(const edge &e)
{
    auto ej = T.begin();
    edges.push_back({e.start, (*ej).helper});
    (*ej).helper = e.start;
    e.helper = e.start;
    T.insert(e);
}

void polygon::handleEnd(const edge &e)
{
    if (e.helper.type == 3)
        edges.push_back({e.end, e.helper});
    T.erase(e);
}

void polygon::handleMerge(const edge &e)
{
    if (e.helper.type == 3)
        edges.push_back({e.end, e.helper});
    T.erase(e);
    auto ej = T.begin();
    if ((*ej).helper.type == 3)
        edges.push_back({e.end, (*ej).helper});
    (*ej).helper = e.end;
}

void polygon::handleRegular(const edge &e_pr, const edge &e)
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
