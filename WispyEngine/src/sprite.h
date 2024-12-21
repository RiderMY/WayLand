#pragma once

#include "memory"
#include "string"

namespace wispy {

struct Sprite {
  Sprite();

  std::shared_ptr<unsigned int> sprite_id;
  unsigned int slices_x, slices_y;
  unsigned int index_x, index_y;
};

}
