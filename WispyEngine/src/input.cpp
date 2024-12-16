#include "input.h"

namespace wispy {

InputManager::InputManager(GameObject *parent) : Property(parent), inputs_() { }

bool InputManager::GetKeyDown(Key key) {
  return inputs_[key];
}

void InputManager::SetKeyDown(Key key, bool is_down) {
  inputs_[key] = is_down;
}

}
