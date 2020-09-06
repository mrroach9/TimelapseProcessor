#pragma once

namespace tlp {
static constexpr double EPS = 1e-10;

inline bool double_eq(double a, double b) {
  return -EPS < a - b && a - b < EPS;
}

}