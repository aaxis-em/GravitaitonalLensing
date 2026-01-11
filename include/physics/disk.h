#ifndef DISK_H
#define DISK_H

#include "../raytracer/vec3.h"
#include <algorithm>
#include <cmath>

inline vec3 temperature_to_rgb(double temp) {
  temp = std::max(1000.0, std::min(40000.0, temp));

  double t = temp / 100.0;
  double r, g, b;

  // Red
  if (t <= 66.0) {
    r = 1.0;
  } else {
    r = t - 60.0;
    r = 1.292936186 * pow(r, -0.1332047592);
    r = std::max(0.0, std::min(1.0, r));
  }

  // Green
  if (t <= 66.0) {
    g = t;
    g = 0.39008157876 * log(g) - 0.63184144378;
  } else {
    g = t - 60.0;
    g = 1.129890861 * pow(g, -0.0755148492);
  }
  g = std::max(0.0, std::min(1.0, g));

  // Blue
  if (t >= 66.0) {
    b = 1.0;
  } else if (t <= 19.0) {
    b = 0.0;
  } else {
    b = t - 10.0;
    b = 0.54320678911 * log(b) - 1.19625408914;
  }
  b = std::max(0.0, std::min(1.0, b));

  return vec3(r, g, b);
}

// Enhanced disk with color based on temperature
inline vec3 disk_color_and_intensity(const vec3 &p, double &intensity) {
  double r = std::sqrt(p.x() * p.x() + p.y() * p.y());
  double z = std::abs(p.z());

  // Disk thickness
  if (z > 0.3) {
    intensity = 0.0;
    return vec3(0, 0, 0);
  }

  // Inner hole (event horizon + photon sphere)
  if (r < 3.0) {
    intensity = 0.0;
    return vec3(0, 0, 0);
  }

  // Outer edge
  if (r > 15.0) {
    intensity = 0.0;
    return vec3(0, 0, 0);
  }
  double temp_base = 8000.0; // Base temperature at r=3
  double temperature = temp_base * std::pow(3.0 / r, 0.75);

  double phi = std::atan2(p.y(), p.x());
  double spiral = std::sin(3.0 * phi + 2.0 * r);

  temperature *= (1.0 + 0.3 * spiral);

  double v_orbital = std::sqrt(1.0 / r); // Keplerian
  vec3 tangent(-p.y(), p.x(), 0);
  tangent = unit_vector(tangent);

  vec3 to_observer = unit_vector(vec3(1, 0, 0));
  double beta = dot(tangent * v_orbital, to_observer);

  double doppler = std::sqrt((1.0 - beta) / (1.0 + beta));
  temperature /= doppler;

  vec3 color = temperature_to_rgb(temperature);

  intensity = std::pow(temperature / temp_base, 4.0) * std::exp(-r / 8.0) *
              std::exp(-8.0 * z);

  intensity *= (1.0 + 0.5 * spiral);
  intensity = std::max(0.0, intensity);

  return color;
}

inline double disk_intensity(const vec3 &p) {
  double intensity;
  disk_color_and_intensity(p, intensity);
  return intensity;
}

#endif
