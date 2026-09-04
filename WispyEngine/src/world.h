#pragma once

#include "camera.h"
#include "game_object.h"
#include "input.h"

namespace wispy {

class World {
public:
  World();
  virtual ~World() = default;

  virtual bool Update(float dt) = 0;

  Camera &GetMainCamera();
  InputManager &GetInputManager();

  void AppendToWorldLog(const std::string &msg);
  std::vector<std::string> GetWorldLogBuffer();

  void SetTargetFPS(unsigned int target_fps);
  unsigned int GetTargetFPS() const;

  void SetCurrentWorldIndex(unsigned long long index);
  unsigned long long GetCurrentWorldIndex() const;

  void SetDebugMode(bool debug_mode);
  bool GetDebugMode() const;

private:
  GameObject main_camera_;
  GameObject input_manager_;

  std::vector<std::string> world_log_buffer_;
  unsigned int target_fps_;
  unsigned long long current_world_index_;
  bool debug_mode_;
};

}
