#pragma once

#include <Windows.h>
#include <algorithm>
#include <unordered_map>

#include "graphics.h"
#include "log.h"
#include "world.h"

namespace wispy {

class Application {
public:
  Application(LPCWSTR name, unsigned int resolution_width, unsigned int resolution_height);
  ~Application();

  void Run(unsigned int target_fps);

  template <typename T>
  void SetCurrentWorld() requires std::is_base_of_v<World, T> {
    current_world_ = std::make_unique<T>();
  }

private:
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  void ActivateFullscreen() const;

  void HandleInput(unsigned long long key, bool is_down);

  void HandleWorldGraphics();

  inline int Minimum(int a, int b) { return a < b ? a : b; }

  static std::unordered_map<HWND, Application *> finder_;

  HWND hwnd_;
  bool is_bad_;

  Log log_;

  Graphics graphics_;
  std::vector<CComPtr<ID2D1Bitmap>> bitmaps_;
  std::vector<CameraSpriteData> camera_sprite_data_;
  bool adjust_to_update_;
  float scale_, fixed_w_, fixed_h_, dis_x_, dis_y_;
  std::vector<DebugDrawer::DrawData> debug_draw_data_;

  std::unique_ptr<World> current_world_;
};

}
