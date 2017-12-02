#include "point.h"
#include <cmath>

point geo2cart(const point& p)
{
        point c;
        c.x=std::cos(p.y)*std::cos(p.x);
        c.y=std::cos(p.y)*std::sin(p.x);
        c.z=std::sin(p.y);

        return c;
}
