#include "point.h"
#include "edge.h"
#include <vector>

#ifndef ELEMENT_H
#define ELEMENT_H

struct elem
{
        size_t index;
        point center_point;
        std::vector<point> vertices;
        std::vector<edge> edges;
        std::vector<elem*> neighbours;
};

bool within(const elem&, const point&);

#endif // ELEMENT_H
