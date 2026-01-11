#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
  vec3 x;   // position
  vec3 v;   // velocity (direction, not speed)
  double h; // angular momentum magnitude

  ray() {}

  ray(const vec3 &origin, const vec3 &direction) {
    x = origin;
    v = unit_vector(direction);
    h = cross(x, v).length();
  }
};

#endif
