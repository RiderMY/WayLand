#include "my_world.h"

#include "collider.h"
#include "sprite_renderer.h"

#include "player.h"

MyWorld::MyWorld() : object_sprite_(), object_(this, { 3.0f, 1.0f }), object_2_(this, { 2.0f, 1.0f }), player_sprite_(), player_(this, { 0.0f, 0.0f }), toggle_debug_(false) {
  object_sprite_ = GetMainCamera().RegisterSprite("src/test_tilemap.png", 1u, 1u);
  object_sprite_.index_x = 0u, object_sprite_.index_y = 1u;
  object_.AddProperty<wispy::SpriteRenderer>(object_sprite_);
  object_.AddProperty<wispy::Collider>(1.0f, 1.0f);
  object_2_.AddProperty<wispy::SpriteRenderer>(object_sprite_);
  //object_2_.AddProperty<wispy::Collider>(1.0f);

  player_sprite_ = GetMainCamera().RegisterSprite("src/test_img.png");
  player_.AddProperty<wispy::SpriteRenderer>(player_sprite_, 1);
  player_.AddProperty<wispy::Collider>(1.0f);
  player_.AddProperty<Player>();

  GetMainCamera().SetUnitSize(16u);
}

bool MyWorld::Update(float dt) {
  if (GetInputManager().GetKeyDown(wispy::kEscape)) return false;

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

  if (GetInputManager().GetKeyDown(wispy::kF)) {
    AppendToWorldLog("(" + std::to_string(player_.GetPosition().x) + "," + std::to_string(player_.GetPosition().y) + ")");
  }

  return true;
}
