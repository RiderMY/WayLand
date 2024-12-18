#include "player.h"

#include "world.h"

Player::Player(wispy::GameObject *parent) : Property(parent) { }

void Player::Update(float dt) {
  int horizontal = GetInputManager().GetKeyDown(wispy::kRight) - GetInputManager().GetKeyDown(wispy::kLeft);
  int vertical = GetInputManager().GetKeyDown(wispy::kDown) - GetInputManager().GetKeyDown(wispy::kUp);

  if (horizontal || vertical) {
    wispy::Vec2 velocity = wispy::Vec2((float) horizontal, (float) vertical);
    velocity /= sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
    velocity *= 5 * dt;

    SetPosition(GetPosition() + velocity);
  }
}

void Player::DebugDraw(wispy::DebugDrawer &dd) {
  dd.DrawRect(4.0f, 3.0f, 1.0f, 4.0f, { 1.0f, 0.0f, 0.0f });
  dd.DrawCircle(10.0f, 6.0f, 0.5f, { 0.0f, 1.0f, 0.0f });
}
