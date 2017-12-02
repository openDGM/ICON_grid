#include "point.h"
#include <Eigen/Dense>
#include <iostream>

struct plane
{
  plane(const point& _p1, const point& _p2, const point& _p3) : p1(_p1), p2(_p2), p3(_p3) {};
  point p1;
  point p2;
  point p3;

  point X(double r, double s)
  {
    return point(p1.x + r*(p2.x-p1.x) + s*(p3.x-p1.x),
                 p1.y + r*(p2.y-p1.y) + s*(p3.y-p1.y),
                 p1.z + r*(p2.z-p1.z) + s*(p3.z-p1.z));
  }
};

struct line
{
  line(const point& _p1, const point& _p2) : p1(_p1), p2(_p2) {};
  point p1;
  point p2;

  point X(double t)
  {
    return point(p1.x + t*(p2.x-p1.x),
                 p1.y + t*(p2.y-p1.y),
                 p1.z + t*(p2.z-p1.z));
  }
};

bool intersection(line L, plane P)
{
  using namespace Eigen;

  Vector3d p1,p2,p3;
  Vector3d u,v;

  p1 << P.p1.x,P.p1.y,P.p1.z;
  p2 << P.p2.x,P.p2.y,P.p2.z;
  p3 << P.p3.x,P.p3.y,P.p3.z;

  u = p2-p1;
  v = p3-p1;

  Vector3d n = u.cross(v);

  Vector3d l1,l2;

  l1 << L.p1.x, L.p1.y, L.p1.z;
  l2 << L.p2.x, L.p2.y, L.p2.z;

  if(n.dot(l2-l1) == 0) return false;

  double r = n.dot(p1-l1)/n.dot(l2-l1);
  auto sp = L.X(r);

  Vector3d w;
  w << sp.x, sp.y, sp.z;
  w = w-p1;

  if(r<0) return false;

  double s = (u.dot(v)*w.dot(v)-v.dot(v)*w.dot(u))/(u.dot(v)*u.dot(v)-u.dot(u)*v.dot(v));
  double t = (u.dot(v)*w.dot(u)-u.dot(u)*w.dot(v))/(u.dot(v)*u.dot(v)-u.dot(u)*v.dot(v));

  return s>=0 && t>=0 && s+t<=1;
}
