#pragma once

namespace wispy {

struct Vec2 {
  Vec2();
  Vec2(float x, float y);
  Vec2(const Vec2 &other);
  void operator=(const Vec2 &other);

  Vec2 operator+(const Vec2 &other) const;
  Vec2 operator-(const Vec2 &other) const;
  Vec2 operator*(const float &other) const;
  Vec2 operator/(const float &other) const;
  void operator+=(const Vec2 &other);
  void operator-=(const Vec2 &other);
  void operator*=(const float &other);
  void operator/=(const float &other);
  bool operator==(const Vec2 &other) const;

  float x, y;
};

}
