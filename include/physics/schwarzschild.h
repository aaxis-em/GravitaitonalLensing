#ifndef SCHWARZSCHILD_H
#define SCHWARZSCHILD_H

#include "../raytracer/ray.h"
#include "../raytracer/vec3.h"
#include <cmath>

inline vec3 schwarzschild_acceleration(const ray &r) {
  double R = r.x.length();
  double factor = -1.5 * r.h * r.h / std::pow(R, 5);
  return r.x * factor;
}

#endif
