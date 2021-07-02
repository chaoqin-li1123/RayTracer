#ifndef IMAGE_UTIL_H
#define IMAGE_UTIL_H
#include <cassert>
#include <cstdint>
#include <fstream>
#include <vector>

#include "vec3.h"

template <typename T>
using Array2D = std::vector<std::vector<T>>;

using color_t = uint8_t;

using Image = Array2D<Color>;

// Hardcode the screen and camera for now.
constexpr double ASPECT_RATIO = 16.0 / 9.0;
constexpr int IMAGE_W = 400;
constexpr int IMAGE_H = IMAGE_W / ASPECT_RATIO;

constexpr double VIEWPORT_H = 2.0;
constexpr double VIEWPORT_W = VIEWPORT_H * ASPECT_RATIO;
constexpr double FOCAL_LEN = 1.0;

constexpr int SAMPLE_RATE = 6;

// For convenience define some of the color utility in image.h
static color_t float2color(float num) {
  return static_cast<color_t>(num * 255.99);
}

struct ImagePrinter {
  /*
static void printPpm(Image const& image, std::string filename) {
  const size_t H = image.size(), W = image.empty() ? 0 : image[0].size();
  std::ofstream file(filename);
  // Print header.
  file << "P3\n" << W << ' ' << H << "\n255\n";
  // Print color of each pixel.
  for (int h = H - 1; h >= 0; h--) {
    for (int w = 0; w <= (int)W; w++) {
      Color const& color = image[h][w];
      color_t r = float2color(color.x()), g = float2color(color.y()),
              b = float2color(color.z());
      file << r << ' ' << g << ' ' << b << '\n';
    }
  }
}
*/

  static void printPpm(Color image[][IMAGE_W], std::string filename) {
    std::ofstream file(filename);
    // Print header.
    file << "P3\n" << IMAGE_W << ' ' << IMAGE_H << "\n255\n";
    // Print color of each pixel.
    for (int h = IMAGE_H - 1; h >= 0; h--) {
      for (int w = 0; w < IMAGE_W; w++) {
        Color color = image[h][w];
        int r = float2color(color.x()), g = float2color(color.y()),
            b = float2color(color.z());
        file << r << ' ' << g << ' ' << b << '\n';
      }
    }

    file.close();
  }
};

#endif