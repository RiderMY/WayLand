#pragma once

#include <memory>

#include "property.h"

namespace wispy {

class Collider : public Property {
public:
  Collider(GameObject *parent, float width, float height);
  Collider(GameObject *parent, float radius);

  ~Collider();

  void Start();

  void Move(Vec2 velocity);

private:
  static std::vector<std::weak_ptr<Collider>> colliders_;

  bool CanMoveTo(float x, float y, std::vector<int> &collisions) const;
  static bool AABBAABBIsColliding(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh);
  static bool CircleCircleIsColliding(float ax, float ay, float ar, float bx, float by, float br);
  static bool AABBCircleIsColliding(float ax, float ay, float aw, float ah, float bx, float by, float br);
  static bool PointCircleIsColliding(float ax, float ay, float bx, float by, float br);
  static float FindCircleCircleVelocityMultiplier(float vx, float vy, float ax, float ay, float ar, float bx, float by, float br);
  static float FindAABBCircleVelocityMultiplier(float vx, float vy, float ax, float ay, float aw, float ah, float bx, float by, float br);

  void DebugDraw(wispy::DebugDrawer &dd);

  enum {
    kRect, kCircle
  } type_;

  union {
    struct {
      float width_, height_;
    };
    float radius_;
  };

  float x_, y_;
  unsigned int collider_id_;
  bool is_colliding_;
};

}
