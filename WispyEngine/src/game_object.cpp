#include "game_object.h"

namespace wispy {

GameObject::GameObject(World *world, const Vec2 &position) : world_(world), position_(position), properties_() { }

World *GameObject::GetWorld() {
  return world_;
}

Vec2 GameObject::GetPosition() const {
  return position_;
}

void GameObject::SetPosition(const Vec2 &v) {
  position_ = v;
}

}
