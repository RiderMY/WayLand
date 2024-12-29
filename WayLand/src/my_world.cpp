#include "my_world.h"

#include "sprite_renderer.h"

#include "player.h"

MyWorld::MyWorld() : player_sprite_(), object_sprite_(), player_(this, { 0.0f, 0.0f }), object_(this, { 1.0f, 0.0f }), toggle_debug_(false) {
  player_sprite_ = GetMainCamera().RegisterSprite("src/test_img.png");
  player_.AddProperty<wispy::SpriteRenderer>();
  player_.GetProperty<wispy::SpriteRenderer>().SetOrder(1);
  player_.GetProperty<wispy::SpriteRenderer>().SetSprite(player_sprite_);
  player_.AddProperty<Player>();

  object_sprite_ = GetMainCamera().RegisterSprite("src/test_tilemap.png", 1u, 1u);
  object_sprite_.index_x = 0u, object_sprite_.index_y = 0u;
  object_.AddProperty<wispy::SpriteRenderer>();
  object_.GetProperty<wispy::SpriteRenderer>().SetOrder(0);
  object_.GetProperty<wispy::SpriteRenderer>().SetSprite(object_sprite_);

  GetMainCamera().SetUnitSize(16u);
}

bool MyWorld::Update(float dt) {
  if (GetInputManager().GetKeyDown(wispy::kEscape)) return false;

  if (GetInputManager().GetKeyDown(wispy::kF)) {
    AppendToWorldLog("do something");
    SetTargetFPS(120u);
  }

  if (GetInputManager().GetKeyDown(wispy::kD)) {
    SetCurrentWorldIndex(0ull);
  }

  if (GetInputManager().GetKeyDown(wispy::kBacktick)) {
    if (!toggle_debug_) {
      SetDebugMode(!GetDebugMode());
      toggle_debug_ = true;
    }
  } else toggle_debug_ = false;

  player_.GetProperty<Player>().Update(dt);

  return true;
}
