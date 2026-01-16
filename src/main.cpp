#include "../include/physics/disk.h"
#include "../include/physics/rk4.h"
#include "../include/raytracer/ray.h"
#include "../include/raytracer/vec3.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

int main() {
  const int image_width = 800;
  const int image_height = 800;

  vec3 camera_pos(25.0, 0.0, 8.0);

  vec3 look_at(0.0, 0.0, 0.0);

  vec3 camera_dir = unit_vector(look_at - camera_pos);

  vec3 camera_right = unit_vector(cross(vec3(0, 0, 1), camera_dir));
  vec3 camera_up = cross(camera_dir, camera_right);

  double fov_scale = 0.8; // Adjust this to zoom in/out

  double dt = 0.02;
  double tmax = 40.0;
  double horizon = 2.0;

  std::ofstream out("bh.ppm");
  out << "P3\n" << image_width << " " << image_height << "\n255\n";

  std::cerr << "Rendering black hole...\n";
  std::cerr << "Resolution: " << image_width << "x" << image_height << "\n";
  std::cerr << "Camera at: (" << camera_pos.x() << ", " << camera_pos.y()
            << ", " << camera_pos.z() << ")\n";

  for (int j = image_height - 1; j >= 0; --j) {
    if (j % 50 == 0) {
      std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
    }

    for (int i = 0; i < image_width; ++i) {
      double u = (i + 0.5) / image_width - 0.5;  // -0.5 to 0.5
      double v = (j + 0.5) / image_height - 0.5; // -0.5 to 0.5

      vec3 ray_dir = camera_dir + camera_right * (u * fov_scale) +
                     camera_up * (v * fov_scale);
      ray_dir = unit_vector(ray_dir);

      ray r(camera_pos, ray_dir);

      vec3 accumulated_color(0, 0, 0);
      double opacity = 1.0;
      bool escaped = true;

      for (double t = 0; t < tmax && opacity > 1e-3; t += dt) {
        double R = r.x.length();

        if (R < horizon) {
          escaped = false;
          break;
        }

        double intensity;
        vec3 disk_color = disk_color_and_intensity(r.x, intensity);

        if (intensity > 0) {
          accumulated_color += disk_color * intensity * opacity;

          opacity *= (1.0 - intensity / 20.0);
        }

        rk4_step(r, dt);
      }

      if (escaped && opacity > 0.5) {
        double R = r.x.length();
        double bg_intensity = 0.05 * std::exp(-0.01 * R);
        accumulated_color +=
            vec3(bg_intensity * 0.7, bg_intensity * 0.8, bg_intensity * 1.0);
      }

      double r_out = std::pow(accumulated_color.x(), 0.8);
      double g_out = std::pow(accumulated_color.y(), 0.8);
      double b_out = std::pow(accumulated_color.z(), 0.8);

      int ir = std::min(255, (int)(r_out * 255.0));
      int ig = std::min(255, (int)(g_out * 255.0));
      int ib = std::min(255, (int)(b_out * 255.0));

      out << ir << " " << ig << " " << ib << "\n";
    }
  }

  out.close();
  std::cerr << "\nDone! Saved to bh.ppm\n";

  return 0;
}
