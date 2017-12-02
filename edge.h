#include "point.h"

#ifndef EDGE_H
#define EDGE_H

struct edge
{
	edge(const point& _p1, const point& _p2, const point& _c) : p1(_p1), p2(_p2), center(_c) {}

	point p1;
	point p2;
	point center;
};

inline bool operator==(const edge& e1, const edge& e2)
{
	return e1.p1 == e2.p1 && e1.p2 == e2.p2 && e1.center == e2.center;
}

#endif /* EDGE_H */
