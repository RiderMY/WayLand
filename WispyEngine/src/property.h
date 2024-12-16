#pragma once

namespace wispy {

class GameObject;

class InputManager;

class Property {
public:
  Property(GameObject *parent);
  virtual ~Property() = default;

  GameObject *GetGameObject();

  virtual void Start();

  InputManager &GetInputManager();

private:
  GameObject *game_object_;
};

}
