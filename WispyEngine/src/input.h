#pragma once

#include <array>

#include "property.h"

namespace wispy {

enum Key {
  kLeft, kRight, kUp, kDown, kEscape, kBacktick, kD, kF, kSize
};

class InputManager : public Property {
public:
  InputManager(GameObject *parent);

  bool GetKeyDown(Key key);
  void SetKeyDown(Key key, bool is_down);

private:
  std::array<bool, Key::kSize> inputs_;
};

}
