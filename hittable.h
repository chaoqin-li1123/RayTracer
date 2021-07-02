#ifndef HITTABLE_H
#define HITTABLE_H
#include <memory>
#include <vector>

#include "ray.h"

struct HitRecord {
  Point p_;
  Direction normal_;
  double t_;
  bool front_face_;
  // set the normal vector to point against the ray for convenience of coloring.
  void setFaceNormal(Ray const& ray, Direction const& outward_normal) {
    front_face_ = dot(ray.direction(), outward_normal);
    normal_ = front_face_ ? outward_normal : -outward_normal;
  }
};

class Hittable {
 public:
  virtual bool hit(Ray const& ray, double t_min, double t_max,
                   HitRecord& hit_record) const = 0;
  virtual ~Hittable() = default;
};

class HittableList : public Hittable {
 public:
  virtual ~HittableList() = default;
  virtual bool hit(Ray const& ray, double t_min, double t_max,
                   HitRecord& hit_record) const override {
    bool hit_any = false;
    double closest_t = t_max;
    HitRecord temp_record;
    // Find the hit point cloest to the camera.
    for (auto const& hittable : hittables) {
      if (hittable->hit(ray, t_min, t_max, temp_record)) {
        hit_any = true;
        if (temp_record.t_ < closest_t) {
          closest_t = temp_record.t_;
          hit_record = temp_record;
        }
      }
    }
    return hit_any;
  }
  void addHittable(std::unique_ptr<Hittable>&& hittable) {
    hittables.push_back(std::move(hittable));
  }

 private:
  std::vector<std::unique_ptr<Hittable>> hittables;
};

#endif