#ifndef CAMERA_H
#define CAMERA_H
#include <array>
#include <cstdint>
#include <fstream>
#include <vector>

#include "world.h"

using color_t = uint8_t;

// Hardcode the screen and camera for now.
constexpr double ASPECT_RATIO = 16.0 / 9.0;
constexpr int IMAGE_W = 1024;
constexpr int IMAGE_H = IMAGE_W / ASPECT_RATIO;

using Image = std::array<std::array<Color, IMAGE_W>, IMAGE_H>;

constexpr int SAMPLE_RATE = 15;

// For convenience define some of the color utility in image.h
static color_t float2color(float num) {
  return static_cast<color_t>(sqrt(num) * 255.99);
}

static double degree_to_radian(double degrees) { return degrees * PI / 180.0; }

struct Camera {
 public:
  Camera(Point look_from, Point look_at, Direction view_up, double vfov,
         double aspect_ratio, double aperture) {
    double theta = degree_to_radian(vfov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    double focus_dist = (look_from - look_at).len();

    z_ = (look_from - look_at) / focus_dist;
    y_ = view_up.normalize();
    x_ = cross(y_, z_);

    origin_ = look_from;
    horizontal_ = focus_dist * viewport_width * x_;
    vertical_ = focus_dist * viewport_height * y_;
    lower_left_ =
        origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - z_ * focus_dist;

    len_radius_ = aperture / 2.0;
  }

  Ray emitRay(double dx, double dy) {
    Direction rd = len_radius_ * Direction::rand_unit_vec_in_xy_plane();
    Direction offset = rd.x() * x_ + rd.y() * y_;
    return Ray(origin_ + offset, lower_left_ + dx * horizontal_ +
                                     dy * vertical_ - origin_ - offset);
  }

 private:
  Point origin_;
  Point lower_left_;
  Direction horizontal_;
  Direction vertical_;
  Direction x_, y_, z_;
  double len_radius_;
};

struct ImagePrinter {
  static void printPpm(Image const& image, std::string filename) {
    std::ofstream file(filename);
    // Print header.
    file << "P3\n" << IMAGE_W << ' ' << IMAGE_H << "\n255\n";
    // Print color of each pixel.
    for (int h = IMAGE_H - 1; h >= 0; h--) {
      for (int w = 0; w < IMAGE_W; w++) {
        Color const& color = image[h][w];
        int r = float2color(color.x()), g = float2color(color.y()),
            b = float2color(color.z());
        file << r << ' ' << g << ' ' << b << '\n';
      }
    }
    file.close();
  }
};

#endif