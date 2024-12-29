#pragma once

#include "world.h"

class MyMenu : public wispy::World {
public:
  MyMenu();

  bool Update(float dt);
};
