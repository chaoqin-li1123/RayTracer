#ifndef WORLD_H
#define WORLD_H

#include <cmath>
#include <memory>

#include "background.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

constexpr int MAX_REFLECTION = 30;

struct World {
  static Color findColor(Ray const& ray, int reflections) {
    if (reflections > MAX_REFLECTION) {
      return Color(0, 0, 0);
    }

    HitRecord hit_record;
    if (world.hit(ray, 1e-3, INF, hit_record)) {
      Ray scattered;
      Color attenuation;
      Color emitted = hit_record.material_->emit(hit_record);
      if (!hit_record.material_->scatter(ray, hit_record, attenuation,
                                         scattered))
        return emitted;
      return findColor(scattered, reflections + 1) * attenuation + emitted;
    }
    return Background::color(ray);
  }

  static void addSphere(std::shared_ptr<Material> material, Point const& center,
                        double radius) {
    world.addHittable(std::make_unique<Sphere>(center, radius, material));
  }
  static void init() {
    // Add ground
    addSphere(
        std::make_shared<Lambertian>(std::make_shared<CheckerTexture>(1000)),
        Point(0, -1000, 0), 1000);

    std::shared_ptr<ImageTexture> blue_texture =
        std::make_shared<ImageTexture>("blue.jpeg");
    std::shared_ptr<ImageTexture> gold_texture =
        std::make_shared<ImageTexture>("gold.jpg");
    std::shared_ptr<ImageTexture> grey_texture =
        std::make_shared<ImageTexture>("grey.jpg");
    std::shared_ptr<ImageTexture> fire_texture =
        std::make_shared<ImageTexture>("fire.jpeg");

    for (int i = -11; i < 11; i++) {
      for (int j = -11; j < 11; j++) {
        int material_lottery = rand() % 100;
        double radius = rand_double(0.1, 0.2);
        Point center(i + rand_double(0, 0.9), radius, j + rand_double(0, 0.9));
        if ((center - Point(6, 0.2, 0)).len() <= 0.9) continue;
        std::shared_ptr<Material> material;
        if (material_lottery < 20) {
          switch (material_lottery % 5) {
            case 0:
              material = std::make_shared<DiffusingLight>(blue_texture, 1.0);
              break;
            case 1:
              material = std::make_shared<Lambertian>(gold_texture);
              break;
            default:
              Color albedo = Color::random();
              material = std::make_shared<DiffusingLight>(albedo, 1.0);
              break;
          }
        } else if (material_lottery < 33) {
          Color albedo = Color::random(0.5, 1);
          double fuzz = rand_double(0, 0.5);
          material = std::make_shared<Metal>(albedo, fuzz);
        } else if (material_lottery < 45) {
          material = std::make_shared<Dielectric>(1.5);
        }
        if (material) addSphere(material, center, radius);
      }
    }

    addSphere(std::make_shared<Dielectric>(1.5), Point(0, 1, 0), 1.0);
    addSphere(std::make_shared<DiffusingLight>(fire_texture, 1.0),
              Point(-6, 1, 0), 1.0);
    addSphere(std::make_shared<DiffusingLight>(blue_texture, 1.0),
              Point(3, 2.5, -3), 0.9);
    addSphere(std::make_shared<DiffusingLight>(gold_texture, 5.0),
              Point(-5, 2.3, 4), 1.5);

    addSphere(std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0),
              Point(6, 1, 0), 1.0);
  }

 private:
  static HittableList world;
};

HittableList World::world;

#endif