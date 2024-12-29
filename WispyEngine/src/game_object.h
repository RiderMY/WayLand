#pragma once

#include "fhl.h"
#include "property.h"
#include "vec_math.h"

namespace wispy {

class World;

class GameObject {
public:
  GameObject(World *world, const Vec2 &position = { 0 });

  World *GetWorld();

  template <typename T>
  void AddProperty() requires std::is_base_of_v<Property, T> {
    properties_.Add(std::make_shared<T>(this));
    properties_.Get<T>().lock()->DoYoThang();
    properties_.Get<T>().lock()->Start();
  }

  template <typename T>
  T &GetProperty() requires std::is_base_of_v<Property, T> {
    return *properties_.Get<T>().lock();
  }

  template <typename T>
  std::weak_ptr<T> GetPropertyWeak() requires std::is_base_of_v<Property, T> {
    return properties_.Get<T>();
  }

  template <typename T>
  void RemoveProperty() requires std::is_base_of_v<Property, T> {
    properties_.Remove<T>();
  }

  Vec2 GetPosition() const;
  void SetPosition(const Vec2 &v);

private:
  World *world_;

  Vec2 position_;
  FHL<Property> properties_;
};

}
