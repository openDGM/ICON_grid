#include "point.h"
#include <utility>
#include <ostream>

#ifndef BOX_H
#define BOX_H

struct box
{
  box(){}
  box(const point& bl, const point& tr) : bottom_left(bl), top_right(tr){}

  point bottom_left;
  point top_right;
};

inline std::ostream& operator<< (std::ostream& os, const box& b) {
    os << "Bottom left: " << b.bottom_left << "Top right: " << b.top_right << '\n';
    return os;
}

inline std::pair<box,box> hsplit(const box& b)
{
  std::pair<box,box> boxes;
  boxes.first=box(b.bottom_left,point(b.top_right.x,(b.top_right.y+b.bottom_left.y)/2));
  boxes.second=box(point(b.bottom_left.x,(b.top_right.y+b.bottom_left.y)/2),b.top_right);
  return boxes;
}

inline std::pair<box,box> vsplit(const box& b)
{
  std::pair<box,box> boxes;
  boxes.first=box(b.bottom_left,point((b.top_right.x+b.bottom_left.x)/2,b.top_right.y));
  boxes.second=box(point((b.top_right.x+b.bottom_left.x)/2,b.bottom_left.y),b.top_right);
  return boxes;
}

inline bool within(const box& b, const point& p)
{
  if(p.y>=b.bottom_left.y && p.x>=b.bottom_left.x && p.y<=b.top_right.y && p.x<=b.top_right.x)
    return true;
  return false;
}

#endif /* BOX_H */
