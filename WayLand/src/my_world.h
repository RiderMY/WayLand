#pragma once

#include "world.h"

class MyWorld : public wispy::World {
public:
  MyWorld();

  bool Update(float dt);

private:
  wispy::Sprite sprite_;
  wispy::GameObject player_;
  wispy::GameObject object_;
};
