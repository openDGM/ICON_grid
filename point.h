#include <ostream>

#ifndef POINT_H
#define POINT_H

struct point
{
	point(){}
	point(const double& _x, const double& _y, const double& _z=1) : x(_x), y(_y), z(_z) {}

	double x;
	double y;
	double z;
};

inline std::ostream& operator<< (std::ostream& os, const point& p) {
    os << p.x << "," << p.y << "," << p.z << ";\n";
    return os;
}

inline bool operator==(const point& p1, const point& p2)
{
	return p1.x==p2.x && p1.y==p2.y && p1.z==p2.z;
}

point geo2cart(const point&);

#endif /* POINT_H */
