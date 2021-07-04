#ifndef MATERIAL_H
#define MATERIAL_H
#include <iostream>

#include "hittable.h"
class Material {
 public:
  virtual ~Material() = default;
  virtual bool scatter(Ray const& ray, HitRecord const& hit_record,
                       Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
 public:
  virtual ~Lambertian() = default;
  Lambertian(const Color& albedo) : albedo_(albedo) {}
  virtual bool scatter(Ray const& ray, HitRecord const& hit_record,
                       Color& attenuation, Ray& scattered) const override {
    Direction scatter_direction =
        hit_record.normal_ + Direction::rand_unit_vec();
    if (scatter_direction.nearZero()) {
      scatter_direction = hit_record.normal_;
    }

    scattered = Ray(hit_record.p_, scatter_direction);
    attenuation = albedo_;
    return true;
  }

 private:
  Color albedo_;
};

class Metal : public Material {
 public:
  Metal(Color const& albedo, double fuzz) : albedo_(albedo), fuzz_(fuzz) {
    assert(fuzz_ <= 1.0);
  }
  virtual ~Metal() = default;
  virtual bool scatter(Ray const& ray, HitRecord const& hit_record,
                       Color& attenuation, Ray& scattered) const override {
    Direction reflected_direction =
        reflect(ray.direction().normalize(), hit_record.normal_);
    scattered = Ray(hit_record.p_,
                    reflected_direction + fuzz_ * Direction::rand_unit_vec());
    attenuation = albedo_;

    return (dot(scattered.direction(), hit_record.normal_) > 0);
  }

 private:
  Color albedo_;
  double fuzz_;
};

class Dielectric : public Material {
 public:
  Dielectric(double refraction_index) : refraction_index_(refraction_index) {}
  virtual ~Dielectric() = default;
  virtual bool scatter(Ray const& ray_in, HitRecord const& hit_record,
                       Color& attenuation, Ray& scattered) const override {
    attenuation = Color(1.0, 1.0, 1.0);
    double refraction_radio =
        hit_record.front_face_ ? (1.0 / refraction_index_) : refraction_index_;
    Direction refracted = refract(ray_in.direction().normalize(),
                                  hit_record.normal_, refraction_radio);
    scattered = Ray(hit_record.p_, refracted);
    return true;
  }

 private:
  double refraction_index_;
};

#endif