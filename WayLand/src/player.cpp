#include "player.h"

#include "collider.h"
#include "world.h"

Player::Player(wispy::GameObject *parent) : Property(parent) { }

void Player::Update(float dt) {
  int horizontal = GetInputManager().GetKeyDown(wispy::kRight) - GetInputManager().GetKeyDown(wispy::kLeft);
  int vertical = GetInputManager().GetKeyDown(wispy::kDown) - GetInputManager().GetKeyDown(wispy::kUp);

  if (horizontal || vertical) {
    wispy::Vec2 velocity = { (float) horizontal, (float) vertical };
    velocity /= sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
    velocity *= 5 * dt;

    GetGameObject()->GetProperty<wispy::Collider>().Move(velocity);
  }
}
