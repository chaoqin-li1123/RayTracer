#ifndef RAY_H
#define RAY_H
#include "vec3.h"

class Ray {
 public:
  // Default to start from (0, 0, 0)
  Ray() = default;
  explicit Ray(Direction dir) : dir_(dir) {}
  Ray(Point origin, Direction dir) : origin_(origin), dir_(dir) {}

  Point origin() const { return origin_; }

  Direction direction() const { return dir_; }

  Point at(double t) const { return origin_ + t * dir_; }

 private:
  Point origin_;
  Direction dir_;
};

#endif