#pragma once

#include "camera.h"
#include "game_object.h"
#include "input.h"

namespace wispy {

class World {
public:
  World();

  virtual bool Update(float dt) = 0;

  InputManager &GetInputManager();
  Camera &GetMainCamera();

private:
  GameObject input_manager_;
  GameObject main_camera_;
};

}
