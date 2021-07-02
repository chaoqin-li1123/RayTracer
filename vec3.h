#ifndef VEC3_H
#define VEC3_H

#include <algorithm>
#include <cmath>
#include <type_traits>

template <typename T>
class Vec3 {
 public:
  // Init all entries to be 0
  Vec3() : v_{} {}

  Vec3(T x, T y, T z) { v_[0] = x, v_[1] = y, v_[2] = z; }

  Vec3(Vec3 const &rhs) {
    v_[0] = rhs.x();
    v_[1] = rhs.y();
    v_[2] = rhs.z();
  }

  Vec3 &operator=(Vec3 const &rhs) {
    v_[0] = rhs.x();
    v_[1] = rhs.y();
    v_[2] = rhs.z();
    return *this;
  }

  T x() const { return v_[0]; }

  T y() const { return v_[1]; }

  T z() const { return v_[2]; }

  T lenSquared() const { return x() * x() + y() * y() + z() * z(); }

  // Considered caching the result of len or lenSquared, that would require
  // storing a bool and a double as member. This optimization is not worthy
  // because the value can always be cached by caller, which doesn't incur any
  // perfomance penalty.
  T len() const {
    static_assert(
        std::is_same<T, double>::value || std::is_same<T, float>::value,
        "Can only compute length of float or double.");
    return sqrt(lenSquared());
  }

  Vec3 operator-() const { return Vec3(-x(), -y(), -z()); }

  Vec3 &operator-=(Vec3 const &rhs) {
    v_[0] -= rhs.x();
    v_[1] -= rhs.y();
    v_[2] -= rhs.z();
    return *this;
  }

  Vec3 &operator+=(Vec3 const &rhs) {
    v_[0] += rhs.x();
    v_[1] += rhs.y();
    v_[2] += rhs.z();
    return *this;
  }

  Vec3 &operator*=(const T scalar) {
    v_[0] *= scalar;
    v_[1] *= scalar;
    v_[2] *= scalar;
    return *this;
  }

  Vec3 &operator/=(const T div) { return (*this) *= (1 / div); }

  Vec3 normalize() { return Vec3(*this) /= len(); }

  static Vec3 rand_unit_vec() {
    return Vec3(rand() % 101 - 50, rand() % 101 - 50, rand() % 101 - 50)
        .normalize();
  }

 private:
  T v_[3];
};

template <typename T>
Vec3<T> operator*(Vec3<T> const &v, T scalar) {
  return Vec3<T>(v.x() * scalar, v.y() * scalar, v.z() * scalar);
}

template <typename T>
Vec3<T> operator*(T scalar, Vec3<T> const &v) {
  return v * scalar;
}

template <typename T>
Vec3<T> operator/(Vec3<T> const &v, T div) {
  return (1 / div) * v;
}

template <typename T>
Vec3<T> operator+(Vec3<T> const &v1, Vec3<T> const &v2) {
  return Vec3<T>(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

template <typename T>
Vec3<T> operator-(Vec3<T> const &v1, Vec3<T> const &v2) {
  return Vec3<T>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

template <typename T>
T dot(Vec3<T> const &v1, Vec3<T> const &v2) {
  return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

template <typename T>
Vec3<T> cross(Vec3<T> const &v1, Vec3<T> const &v2) {
  return Vec3<T>(v1.y() * v2.z() - v1.z() * v1.y(),
                 v1.z() * v2.x() - v1.x() * v2.z(),
                 v1.x() * v2.y() - v1.y() * v2.z());
}

using Point = Vec3<double>;
using Direction = Vec3<double>;
// Considered using u_int8_t for color type here, but there is concern
// about losing precision in some intermediate operation.
// Each element should be a float between [0.0, 1.0]
using Color = Vec3<float>;

#endif