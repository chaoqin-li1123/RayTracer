#ifndef WORLD_H
#define WORLD_H

#include <cmath>

#include "background.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

constexpr int MAX_REFLECTION = 50;

struct World {
  static Color findColor(Ray const& ray, int reflections) {
    if (reflections > MAX_REFLECTION) {
      return Color(0, 0, 0);
    }

    HitRecord hit_record;
    if (world.hit(ray, 1e-3, INF, hit_record)) {
      Ray scattered;
      Color attenuation;
      if (hit_record.material_->scatter(ray, hit_record, attenuation,
                                        scattered))
        return findColor(scattered, reflections + 1) * attenuation;
      return Color(0, 0, 0);
    }
    return Background::color(ray);
  }
  static void init() {
    std::shared_ptr<Lambertian> material_ground =
        std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    std::shared_ptr<Dielectric> material_center =
        std::make_shared<Dielectric>(1.5);

    std::shared_ptr<Dielectric> material_left =
        std::make_shared<Dielectric>(1.5);
    std::shared_ptr<Metal> material_right =
        std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.addHittable(std::make_unique<Sphere>(Point(0.0, -100.5, -1.0), 100.0,
                                               material_ground));
    world.addHittable(
        std::make_unique<Sphere>(Point(0.0, 0.0, -1.0), 0.5, material_center));

    world.addHittable(
        std::make_unique<Sphere>(Point(-1.0, 0.0, -1.0), 0.5, material_left));
    world.addHittable(
        std::make_unique<Sphere>(Point(1.0, 0.0, -1.0), 0.5, material_right));
  }

 private:
  static HittableList world;
};

HittableList World::world;

#endif