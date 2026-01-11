#ifndef RK4_H
#define RK4_H

#include "../raytracer/ray.h"
#include "schwarzschild.h"

inline void rk4_step(ray &r, double dt) {

  vec3 k1x = r.v;
  vec3 k1v = schwarzschild_acceleration(r);

  ray r2 = r;
  r2.x += k1x * (dt * 0.5);
  r2.v += k1v * (dt * 0.5);
  vec3 k2x = r2.v;
  vec3 k2v = schwarzschild_acceleration(r2);

  ray r3 = r;
  r3.x += k2x * (dt * 0.5);
  r3.v += k2v * (dt * 0.5);
  vec3 k3x = r3.v;
  vec3 k3v = schwarzschild_acceleration(r3);

  ray r4 = r;
  r4.x += k3x * dt;
  r4.v += k3v * dt;
  vec3 k4x = r4.v;
  vec3 k4v = schwarzschild_acceleration(r4);

  r.x += (k1x + 2 * k2x + 2 * k3x + k4x) * (dt / 6.0);
  r.v += (k1v + 2 * k2v + 2 * k3v + k4v) * (dt / 6.0);
}

#endif
