#ifndef SPHERE_H
#define SPHERE_H
#include <cassert>
#include <memory>

#include "material.h"

class Sphere : public Hittable {
 public:
  Sphere(Point const& center, double radius, std::shared_ptr<Material> material)
      : center_(center), radius_(radius), material_(material) {}

  Point center() const { return center_; }
  bool hit(Ray const& ray, double t_min, double t_max,
           HitRecord& hit_record) const override {
    Direction oc = ray.origin() - center_;
    double a = dot(ray.direction(), ray.direction());
    double b = 2.0 * dot(oc, ray.direction());
    double c = dot(oc, oc) - radius_ * radius_;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
      return false;
    }
    double root = (-b - sqrt(discriminant)) / (2.0 * a);
    if (root < t_min || root > t_max) {
      root = (-b + sqrt(discriminant)) / (2.0 * a);
      if (root < t_min || root > t_max) return false;
    }
    hit_record.t_ = root;
    hit_record.p_ = ray.at(hit_record.t_);
    Direction outward_normal = (hit_record.p_ - center_) / radius_;
    hit_record.setFaceNormal(ray, outward_normal);
    hit_record.material_ = material_;

    return true;
  }

 private:
  Point center_;
  double radius_;
  std::shared_ptr<Material> material_;
};

#endif