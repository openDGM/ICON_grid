#include "element.h"
#include "geometry.h"
#include <math.h>

bool within(const elem& e, const point& pt)
{
  plane P(geo2cart(e.vertices[0]),
          geo2cart(e.vertices[1]),
          geo2cart(e.vertices[2]));
  line L(point(0,0,0),geo2cart(pt));
  return intersection(L,P);
}
