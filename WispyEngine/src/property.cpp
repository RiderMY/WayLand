#include "property.h"

#include "world.h"

namespace wispy {

Property::Property(GameObject *parent) : game_object_(parent), debug_draw_id_(0u) { }

Property::~Property() {
  if (&(game_object_->GetWorld()->GetMainCamera()) != nullptr) game_object_->GetWorld()->GetMainCamera().RemoveDebugDraw(debug_draw_id_);
}

GameObject *Property::GetGameObject() {
  return game_object_;
}

void Property::DoYoThang() {
  debug_draw_id_ = game_object_->GetWorld()->GetMainCamera().AddDebugDraw(std::bind(&Property::DebugDraw, this, std::placeholders::_1));
}

void Property::Start() { }

InputManager &Property::GetInputManager() {
  return game_object_->GetWorld()->GetInputManager();
}

Vec2 Property::GetPosition() const {
  return game_object_->GetPosition();
}

void Property::SetPosition(const Vec2 &v) {
  game_object_->SetPosition(v);
}

void Property::DebugDraw(DebugDrawer &dd) { }

}
