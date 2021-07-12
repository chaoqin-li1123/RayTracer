#ifndef TEXTTURE_H
#define TEXTTURE_H
#include <cassert>
#include <cmath>
#include <string_view>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "vec3.h"

class Texture {
 public:
  // Pass in a unit normal vector and return the corresponding color mapping of
  // the object.
  virtual Color getColor(Point p) = 0;
  virtual ~Texture() = default;

 protected:
  static std::pair<double, double> xyz2uv(Point const& p) {
    double v = acos(p.y()) / PI;
    double u = (atan2(-p.z(), p.x()) + PI) / (2 * PI);
    return {u, v};
  }
};

class CheckerTexture : public Texture {
 public:
  CheckerTexture(double freq) : freq_(freq) {}
  virtual ~CheckerTexture() = default;
  virtual Color getColor(Point p) override {
    p *= freq_;
    if ((isOdd(p.x()) * isOdd(p.y()) * isOdd(p.z())) == 1) {
      return Color(1.0, 1.0, 1.0);
    }
    return Color(0.0, 0.0, 0.0);
  }

 private:
  // return 1 if x is odd, -1 if x is even.
  int isOdd(int x) {
    if (x % 2 != 0) return 1;
    return -1;
  }
  double freq_;
};

class ConstTexture : public Texture {
 public:
  ConstTexture(Color const& color) : color_(color) {}
  virtual ~ConstTexture() = default;
  virtual Color getColor(Point p) override { return color_; }

 private:
  Color color_;
};

class ImageTexture : public Texture {
 public:
  ImageTexture(const char* filename) {
    int components_per_pixel = bytes_per_pixel;
    data = stbi_load(filename, &width, &height, &components_per_pixel,
                     components_per_pixel);
  }
  virtual Color getColor(Point p) override {
    auto [u, v] = Texture::xyz2uv(p);
    int i = std::min(static_cast<int>(u * width), width - 1);
    int j = std::min(static_cast<int>(v * height), height - 1);

    constexpr double color_scale = 1.0 / 255.0;
    unsigned char* pixel = data + (j * width + i) * bytes_per_pixel;
    return Color(color_scale * pixel[0], color_scale * pixel[1],
                 color_scale * pixel[2]);
  }

 private:
  int width, height;
  unsigned char* data;
  static constexpr int bytes_per_pixel = 3;
};

#endif