#pragma once

#include "memory"
#include "string"

namespace wispy {

struct Sprite {
  Sprite();

  std::shared_ptr<unsigned int> sprite_id;
};

}
