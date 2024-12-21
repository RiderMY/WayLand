#include "sprite.h"

namespace wispy {

Sprite::Sprite() : sprite_id(std::make_shared<unsigned int>()), slices_x(0u), slices_y(0u), index_x(0u), index_y(0u) { }

}
