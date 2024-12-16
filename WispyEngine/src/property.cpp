#include "property.h"

#include "world.h"

namespace wispy {

Property::Property(GameObject *parent) : game_object_(parent) { }

GameObject *Property::GetGameObject() {
  return game_object_;
}

void Property::Start() { }

InputManager &Property::GetInputManager() {
  return game_object_->GetWorld()->GetInputManager();
}

}
