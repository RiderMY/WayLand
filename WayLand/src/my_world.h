#pragma once

#include "world.h"

class MyWorld : public wispy::World {
public:
  MyWorld();

  bool Update(float dt);

private:
  wispy::Sprite player_sprite_;
  wispy::Sprite object_sprite_;
  wispy::GameObject player_;
  wispy::GameObject object_;
  
  bool toggle_debug_;
};
