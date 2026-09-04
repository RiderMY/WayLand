#pragma once

#include "world.h"

class MyWorld : public wispy::World {
public:
  MyWorld();

  bool Update(float dt);

private:
  wispy::Sprite object_sprite_;
  wispy::GameObject object_;
  wispy::GameObject object_2_;

  wispy::Sprite player_sprite_;
  wispy::GameObject player_;
  
  bool toggle_debug_;
};
