#include "collider.h"

#include <bit>

#include "game_math.h"
#include "world.h"

namespace wispy {

std::vector<std::weak_ptr<Collider>> Collider::colliders_;

Collider::Collider(GameObject *parent, float width, float height) : Property(parent), type_(kRect), width_(width), height_(height), x_(parent->GetPosition().x), y_(parent->GetPosition().y), collider_id_(0u), is_colliding_(false) { }

Collider::Collider(GameObject *parent, float radius) : Property(parent), type_(kCircle), radius_(radius), x_(parent->GetPosition().x), y_(parent->GetPosition().y), collider_id_(0u), is_colliding_(false) { }

Collider::~Collider() {
  colliders_.erase(colliders_.begin() + (collider_id_ - 1u));
}

void Collider::Start() {
  if (collider_id_ == 0u) {
    auto property = GetGameObject()->GetPropertyWeak<Collider>();
    if (property.expired()) {
      GetGameObject()->GetWorld()->AppendToWorldLog("Collider was prematurely removed from GameObject");
    } else {
      colliders_.push_back(property);
      collider_id_ = static_cast<unsigned int>(colliders_.size());
    }
  }
}

void Collider::Move(Vec2 velocity) {
  if (velocity.x == 0 && velocity.y == 0) return;

  std::vector<int> collisions;
  is_colliding_ = false;

  if (!CanMoveTo(x_ + velocity.x, y_ + velocity.y, collisions)) {
    switch (type_) {
    case kRect:
    {
      if (auto c = colliders_[collisions[0]].lock()) {
        if (c->type_ == kRect) {
          // Horizontal Movement
          if (velocity.x != 0) {
            if (!CanMoveTo(x_ + velocity.x, y_, collisions)) {
              if (auto c = colliders_[collisions[0]].lock()) {
                if (velocity.x > 0) x_ = c->x_ - width_;
                else x_ = c->x_ + c->width_;
              }
            } else x_ += velocity.x;
          }

          collisions.clear();

          // Vertical Movement
          if (velocity.y != 0) {
            if (!CanMoveTo(x_, y_ + velocity.y, collisions)) {
              if (auto c = colliders_[collisions[0]].lock()) {
                if (velocity.y > 0) y_ = c->y_ - height_;
                else y_ = c->y_ + c->height_;
              }
            } else y_ += velocity.y;
          }
        } else if (c->type_ == kCircle) {
          if (velocity.x == 0.0f || velocity.y == 0.0f) {
            float v = FindAABBCircleVelocityMultiplier(velocity.x, velocity.y, x_, y_, width_, height_, c->x_, c->y_, c->radius_);
            x_ += velocity.x * v, y_ += velocity.y * v;
          } else if ((c->x_ - width_ <= x_ && x_ <= c->x_) || (Sign(c->x_ - x_) == -Sign(velocity.x) && Sign(c->y_ - y_) == Sign(velocity.y))) {
            // x then y
            float v = FindAABBCircleVelocityMultiplier(velocity.x, 0, x_, y_, width_, height_, c->x_, c->y_, c->radius_);
            x_ += velocity.x * v;
            v = FindAABBCircleVelocityMultiplier(0, velocity.y, x_, y_, width_, height_, c->x_, c->y_, c->radius_);
            y_ += velocity.y * v;
          } else if ((c->y_ - height_ <= y_ && y_ <= c->y_) || (Sign(c->x_ - x_) == Sign(velocity.x) && Sign(c->y_ - y_) == -Sign(velocity.y))) {
            // y then x
            float v = FindAABBCircleVelocityMultiplier(0, velocity.y, x_, y_, width_, height_, c->x_, c->y_, c->radius_);
            y_ += velocity.y * v;
            v = FindAABBCircleVelocityMultiplier(velocity.x, 0, x_, y_, width_, height_, c->x_, c->y_, c->radius_);
            x_ += velocity.x * v;
          } else {
            float v = FindAABBCircleVelocityMultiplier(velocity.x, velocity.y, x_, y_, width_, height_, c->x_, c->y_, c->radius_);
            x_ += velocity.x * v, y_ += velocity.y * v;
          }
        }
      }
    }
    break;
    case kCircle:
    {
      float min_v = std::numeric_limits<float>::max(), v = 0.0f;
      std::shared_ptr<Collider> col;
      bool b = false;

      for (int i = 0; i < collisions.size(); ++i) {
        if (auto c = colliders_[collisions[i]].lock()) {
          if (c->type_ == kCircle) {
            if (x_ == c->x_ || y_ == c->y_ || Sign(velocity.x * (x_ - c->x_)) == -Sign(velocity.y * (y_ - c->y_))) {
              if (Sign(velocity.x) == Sign(x_ - c->x_)) {
                // x precedes y
                if (velocity.y) v = Min(FindCircleCircleVelocityMultiplier(0.0f, velocity.y, x_ + velocity.x, y_, radius_, c->x_, c->y_, c->radius_), 1.0f);
              } else {
                // y precedes x
                if (velocity.x) v = Min(FindCircleCircleVelocityMultiplier(velocity.x, 0.0f, x_, y_ + velocity.y, radius_, c->x_, c->y_, c->radius_), 1.0f);
              }
            } else v = FindCircleCircleVelocityMultiplier(velocity.x, velocity.y, x_, y_, radius_, c->x_, c->y_, c->radius_);

            if (v < min_v) min_v = v, col = c;
          } else if (c->type_ == kRect) {
            if (velocity.x == 0.0f || velocity.y == 0.0f) {
              v = FindAABBCircleVelocityMultiplier(-velocity.x, -velocity.y, c->x_, c->y_, c->width_, c->height_, x_, y_, radius_);
              x_ += velocity.x * v, y_ += velocity.y * v;
            } else if ((x_ - c->width_ <= c->x_ && c->x_ <= x_) || (Sign(x_ - c->x_) == -Sign(velocity.x) && Sign(y_ - c->y_) == Sign(velocity.y))) {
              // x then y
              v = FindAABBCircleVelocityMultiplier(-velocity.x, 0, c->x_, c->y_, c->width_, c->height_, x_, y_, radius_);
              x_ += velocity.x * v;
              v = FindAABBCircleVelocityMultiplier(0, -velocity.y, c->x_, c->y_, c->width_, c->height_, x_, y_, radius_);
              y_ += velocity.y * v;
            } else if ((y_ - c->height_ <= c->y_ && c->y_ <= y_) || (Sign(x_ - c->x_) == Sign(velocity.x) && Sign(y_ - c->y_) == -Sign(velocity.y))) {
              // y then x
              v = FindAABBCircleVelocityMultiplier(0, -velocity.y, c->x_, c->y_, c->width_, c->height_, x_, y_, radius_);
              y_ += velocity.y * v;
              v = FindAABBCircleVelocityMultiplier(-velocity.x, 0, c->x_, c->y_, c->width_, c->height_, x_, y_, radius_);
              x_ += velocity.x * v;
            } else {
              v = FindAABBCircleVelocityMultiplier(-velocity.x, -velocity.y, c->x_, c->y_, c->width_, c->height_, x_, y_, radius_);
              x_ += velocity.x * v, y_ += velocity.y * v;
            }
            b = true;
          }
        }
      }

      if (!b) {
        if (x_ == col->x_ || y_ == col->y_ || Sign(velocity.x * (x_ - col->x_)) == -Sign(velocity.y * (y_ - col->y_))) {
          if (Sign(velocity.x) == Sign(x_ - col->x_)) x_ += velocity.x, y_ += velocity.y * min_v;
          else x_ += velocity.x * min_v, y_ += velocity.y;
        } else x_ += velocity.x * min_v, y_ += velocity.y * min_v;
      }
    }
    break;
    }
  } else x_ += velocity.x, y_ += velocity.y;

  GetGameObject()->SetPosition({ x_, y_ });
}

bool Collider::CanMoveTo(float x, float y, std::vector<int> &collisions) const {
  bool is_colliding = false, result = false;

  for (int i = 0; i < colliders_.size(); ++i) {
    if (i + 1u == collider_id_) continue;
    if (auto col = colliders_[i].lock()) {
      result = false;

      switch (type_) {
      case kRect:
        switch (col->type_) {
        case kRect:
          result = AABBAABBIsColliding(x, y, width_, height_, col->x_, col->y_, col->width_, col->height_);
          break;
        case kCircle:
          result = AABBCircleIsColliding(x, y, width_, height_, col->x_, col->y_, col->radius_);
          break;
        }
        break;
      case kCircle:
        switch (col->type_) {
        case kRect:
          result = AABBCircleIsColliding(col->x_, col->y_, col->width_, col->height_, x, y, radius_);
          break;
        case kCircle:
          result = CircleCircleIsColliding(x, y, radius_, col->x_, col->y_, col->radius_);
          break;
        }
        break;
      }

      if (result) {
        is_colliding = true;
        collisions.push_back(i);
      }
    } else {
      // TODO: HANDLE ERROR
    }
  }

  return !is_colliding;
}

bool Collider::AABBAABBIsColliding(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
  return Min(ax, bx) > Max(ax - bw, bx - aw) && Min(ay, by) > Max(ay - bh, by - ah);
}

bool Collider::CircleCircleIsColliding(float ax, float ay, float ar, float bx, float by, float br) {
  return AABBAABBIsColliding(ax - ar, ay - ar, ar + ar, ar + ar, bx - br, by - br, br + br, br + br) && (ar + br) * (ar + br) > (bx - ax) * (bx - ax) + (by - ay) * (by - ay);
}

bool Collider::AABBCircleIsColliding(float ax, float ay, float aw, float ah, float bx, float by, float br) {
  return AABBAABBIsColliding(ax, ay, aw, ah, bx - br, by - br, br + br, br + br)
    && (PointCircleIsColliding(ax, ay, bx, by, br)
      || PointCircleIsColliding(ax + aw, ay, bx, by, br)
      || PointCircleIsColliding(ax + aw, ay + ah, bx, by, br)
      || PointCircleIsColliding(ax, ay + ah, bx, by, br)
      || (ax < bx && bx < ax + aw) || (ay < by && by < ay + ah));
}

bool Collider::PointCircleIsColliding(float ax, float ay, float bx, float by, float br) {
  return (ax - bx) * (ax - bx) + (ay - by) * (ay - by) < br * br;
}

float Collider::FindCircleCircleVelocityMultiplier(float vx, float vy, float ax, float ay, float ar, float bx, float by, float br) {
  float dx = ax - bx, dy = ay - by, r = ar + br;
  float vp = vx * dx + vy * dy;
  float R = dx * dx + dy * dy - r * r;
  float v = vx * vx + vy * vy; // magnitude squared of velocity
  return (-vp + Sign(vp) * Sqrt(vp * vp - v * R)) / v;
}

float Collider::FindAABBCircleVelocityMultiplier(float vx, float vy, float ax, float ay, float aw, float ah, float bx, float by, float br) {
  Vec2 velocity = { vx, vy };
  float v2 = Dot(velocity, velocity);
  Vec2 d = { ax - bx, ay - by };
  std::array<Vec2, 4> e = { };
  e[0] = { 0.0f, 0.0f }, e[1] = { aw, 0.0f }, e[2] = { aw, ah }, e[3] = { 0.0f, ah };
  float r2 = (br + 0.001f) * (br + 0.001f); // Add cushioning to avoid entering the if statement
  float vc = 1.0f, D = 0.0f;
  for (int i = 0; i < 4; ++i) {
    D = -Cross(velocity, d + e[i]) * Cross(velocity, d + e[i]) + r2 * v2;
    if (D >= 0) vc = Min(vc, (-Dot(velocity, d + e[i]) - Sqrt(D)) / v2);
  }

  // collision with edges of aabb
  if (AABBCircleIsColliding(ax + vc * vx, ay + vc * vy, aw, ah, bx, by, br)) {
    float vex = std::numeric_limits<float>::max();
    float vey = std::numeric_limits<float>::max();
    if (vx != 0.0f) {
      vex = (bx - ax + (vx > 0.0f ? -br - aw : br)) / vx;
    }
    if (vy != 0.0f) {
      vey = (by - ay + (vy > 0.0f ? -br - ah : br)) / vy;
    }
    return Max(0.0f, Min(vex, vey));
  }

  // If anything's gonna break, it'll be this
  if (vc < -0.1f) return 1.0f; // Stops the player from teleporting back when corners will hit cushion
  if (vc < 0.0f) return 0.0f; // Apparently, a lot of the values for vc are negative numbers really close to 0
  return vc;
}

void Collider::DebugDraw(wispy::DebugDrawer &dd) {
  switch (type_) {
  case kRect:
    dd.DrawRect(GetPosition().x, GetPosition().y, width_, height_, { is_colliding_ ? 0.0f : 1.0f , is_colliding_ ? 1.0f : 0.0f, 0.0f });
    break;
  case kCircle:
    dd.DrawCircle(GetPosition().x, GetPosition().y, radius_, { is_colliding_ ? 0.0f : 1.0f , is_colliding_ ? 1.0f : 0.0f, 0.0f });
    break;
  }
}

}
