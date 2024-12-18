#pragma once

#include "debug_draw.h"
#include "vec_math.h"

namespace wispy {

class GameObject;

class InputManager;

class Property {
public:
  Property(GameObject *parent);
  virtual ~Property() = 0;

  GameObject *GetGameObject();

  void DoYoThang();
  virtual void Start();

  InputManager &GetInputManager();

  Vec2 GetPosition() const;
  void SetPosition(const Vec2 &v);

  virtual void DebugDraw(DebugDrawer &dd);

private:
  GameObject *game_object_;

  unsigned int debug_draw_id_;
  bool need_cleanup_;
  bool is_camera_;
};

}
