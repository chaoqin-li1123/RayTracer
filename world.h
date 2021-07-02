#ifndef WORLD_H
#define WORLD_H
#include <cassert>
#include <cfloat>
#include <cmath>

#include "background.h"
#include "hittable.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

constexpr double INF = DBL_MAX;
constexpr double PI = M_PI;
constexpr int MAX_REFLECTION = 50;

struct World {
  static Color findColor(Ray const& ray, int reflections) {
    if (reflections > MAX_REFLECTION) {
      return Color(0, 0, 0);
    }

    HitRecord hit_record;
    if (world.hit(ray, 0, INF, hit_record)) {
      Point target =
          hit_record.p_ + hit_record.normal_ + Direction::rand_unit_vec();
      return findColor(Ray(hit_record.p_, target - hit_record.p_),
                       reflections + 1) *
             0.5f;
    }
    return Background::color(ray);
  }
  static void init() {
    world.addHittable(std::make_unique<Sphere>(Point(0, 0, -1), 0.5));
    world.addHittable(std::make_unique<Sphere>(Point(0, -100.5, -1), 100));
  }

 private:
  static HittableList world;
};

HittableList World::world;

#endif