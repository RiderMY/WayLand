#include "world.h"

namespace wispy {

World::World() : main_camera_(this), input_manager_(this) {
  main_camera_.AddProperty<Camera>();
  input_manager_.AddProperty<InputManager>();
}

Camera &World::GetMainCamera() {
  return main_camera_.GetProperty<Camera>();
}

InputManager &World::GetInputManager() {
  return input_manager_.GetProperty<InputManager>();
}

}
