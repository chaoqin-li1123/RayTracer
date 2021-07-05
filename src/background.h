#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "ray.h"

struct Background {
  static Color color(Ray const& ray) {
    static const Color white(1.0, 1.0, 1.0), blue(0.5, 0.7, 1.0);
    float blend_factor = (ray.direction().normalize().y() + 1.0) * 0.5;
    return (1.0f - blend_factor) * white + blend_factor * blue;
  }
};

#endif