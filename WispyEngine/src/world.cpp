#include "world.h"

namespace wispy {

World::World() : input_manager_(this), main_camera_(this) {
  input_manager_.AddProperty<InputManager>();
  main_camera_.AddProperty<Camera>();
}

InputManager &World::GetInputManager() {
  return input_manager_.GetProperty<InputManager>();
}

Camera &World::GetMainCamera() {
  return main_camera_.GetProperty<Camera>();
}

}
