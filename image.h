#ifndef IMAGE_UTIL_H
#define IMAGE_UTIL_H
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <vector>

#include "vec3.h"

template <typename T>
using Array2D = std::vector<std::vector<T>>;

using color_t = uint8_t;

// Hardcode the screen and camera for now.
constexpr double ASPECT_RATIO = 16.0 / 9.0;
constexpr int IMAGE_W = 400;
constexpr int IMAGE_H = IMAGE_W / ASPECT_RATIO;

using Image = std::array<std::array<Color, IMAGE_W>, IMAGE_H>;

constexpr double VIEWPORT_H = 2.0;
constexpr double VIEWPORT_W = VIEWPORT_H * ASPECT_RATIO;
constexpr double FOCAL_LEN = 1.0;

constexpr int SAMPLE_RATE = 6;

// For convenience define some of the color utility in image.h
static color_t float2color(float num) {
  return static_cast<color_t>(sqrt(num) * 255.99);
}

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