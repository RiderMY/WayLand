#pragma once

#include "camera.h"
#include "game_object.h"
#include "input.h"

namespace wispy {

class World {
public:
  World();

  virtual bool Update(float dt) = 0;

  Camera &GetMainCamera();
  InputManager &GetInputManager();

private:
  GameObject main_camera_;
  GameObject input_manager_;
};

}
