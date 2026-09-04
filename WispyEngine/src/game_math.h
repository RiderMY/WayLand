#pragma once

#include <cmath>
#include <concepts>

namespace wispy {

template <typename T>
T Max(T a, T b) requires std::is_arithmetic_v<T> {
  return a > b ? a : b;
}

template <typename T>
T Min(T a, T b) requires std::is_arithmetic_v<T> {
  return a < b ? a : b;
}

float Sqrt(float f) {
  return std::sqrtf(f);
}

float SafeSqrt(float f) {
  return Sqrt(Max(f, 0.0f));
}

float Sqrt4Min(float f) {
  return f < 0.0f ? std::numeric_limits<float>::min() : Sqrt(f);
}

float Dist(float x0, float y0, float x1, float y1) {
  return Sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

float Sign(float f) {
  return static_cast<float>((f > 0.0f) - (f < 0.0f));
}

float Abs(float f) {
  return std::abs(f);
}

}
