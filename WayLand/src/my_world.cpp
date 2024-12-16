#include "my_world.h"

#include "sprite_renderer.h"

#include "player.h"

MyWorld::MyWorld() : sprite_(), player_(this, wispy::Vec2(0.0f, 0.0f)), object_(this, wispy::Vec2(1.0f, 0.0f)) {
  sprite_ = GetMainCamera().RegisterSprite("src/test_img.png");

  player_.AddProperty<wispy::SpriteRenderer>();
  player_.GetProperty<wispy::SpriteRenderer>().SetOrder(1);
  player_.GetProperty<wispy::SpriteRenderer>().SetSprite(sprite_);
  player_.AddProperty<Player>();

  object_.AddProperty<wispy::SpriteRenderer>();
  object_.GetProperty<wispy::SpriteRenderer>().SetSprite(sprite_);

  GetMainCamera().SetUnitSize(16);
}

bool MyWorld::Update(float dt) {
  if (GetInputManager().GetKeyDown(wispy::kEscape)) return false;

  player_.GetProperty<Player>().Update(dt);

  return true;
}
