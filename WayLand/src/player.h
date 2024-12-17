#pragma once

#include "input.h"
#include "property.h"

class Player : public wispy::Property {
public:
  Player(wispy::GameObject *parent);

  void Update(float dt);

  void DebugDraw(wispy::DebugDrawer &dd);
};
