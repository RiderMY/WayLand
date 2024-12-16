#include "vec_math.h"

namespace wispy {

Vec2::Vec2() : x(0), y(0) { }

Vec2::Vec2(float x, float y) : x(x), y(y) { }

Vec2::Vec2(const Vec2 &other) {
  x = other.x;
  y = other.y;
}

void Vec2::operator=(const Vec2 &other) {
  x = other.x;
  y = other.y;
}

Vec2 Vec2::operator+(const Vec2 &other) const {
  Vec2 tmp(x + other.x, y + other.y);
  return tmp;
}

Vec2 Vec2::operator-(const Vec2 &other) const {
  Vec2 tmp(x - other.x, y - other.y);
  return tmp;
}

Vec2 Vec2::operator*(const float &other) const {
  Vec2 tmp(x * other, y * other);
  return tmp;
}

Vec2 Vec2::operator/(const float &other) const {
  Vec2 tmp(x / other, y / other);
  return tmp;
}

void Vec2::operator+=(const Vec2 &other) {
  x += other.x;
  y += other.y;
}

void Vec2::operator-=(const Vec2 &other) {
  x -= other.x;
  y -= other.y;
}

void Vec2::operator*=(const float &other) {
  x *= other;
  y *= other;
}

void Vec2::operator/=(const float &other) {
  x /= other;
  y /= other;
}

bool Vec2::operator==(const Vec2 &other) const {
  return x == other.x && y == other.y;
}

}
