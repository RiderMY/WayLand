#include "world.h"

namespace wispy {

World::World() : main_camera_(this), input_manager_(this), world_log_buffer_(), target_fps_(0u), current_world_index_(0u) {
  main_camera_.AddProperty<Camera>();
  input_manager_.AddProperty<InputManager>();
}

Camera &World::GetMainCamera() {
  return main_camera_.GetProperty<Camera>();
}

InputManager &World::GetInputManager() {
  return input_manager_.GetProperty<InputManager>();
}

void World::AppendToWorldLog(const std::string &msg) {
  world_log_buffer_.push_back(msg);
}

std::vector<std::string> World::GetWorldLogBuffer() {
  std::vector<std::string > buffer;

  for (int i = 0; i < world_log_buffer_.size(); ++i) {
    buffer.push_back(world_log_buffer_[i]);
  }

  world_log_buffer_.clear();

  return buffer;
}

void World::SetTargetFPS(unsigned int target_fps) {
  target_fps_ = target_fps;
}

unsigned int World::GetTargetFPS() const {
  return target_fps_;
}

void World::SetCurrentWorldIndex(unsigned long long index) {
  current_world_index_ = index;
}

unsigned long long World::GetCurrentWorldIndex() const {
  return current_world_index_;
}

}
