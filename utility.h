#ifndef UTILITY_H
#define UTILITY_H
#include <cfloat>
#include <cmath>
#include <cstdlib>

constexpr double INF = DBL_MAX;
constexpr double PI = M_PI;

double rand_double() { return rand() / (RAND_MAX + 1.0); }

double rand_double(double min, double max) {
  return min + (max - min) * rand_double();
}

#endif