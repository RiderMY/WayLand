#include "application.h"

#include <chrono>

namespace wispy {

std::unordered_map<HWND, Application *> Application::finder_;

Application::Application(LPCWSTR name, unsigned int resolution_width, unsigned int resolution_height) : hwnd_(NULL), is_bad_(false), log_(), graphics_(), bitmaps_(), camera_sprite_data_(), adjust_to_update_(true), scale_(0.0f), fixed_w_(0.0f), fixed_h_(0.0f), dis_x_(0.0f), dis_y_(0.0f), current_world_(nullptr) {
  // Register Window Class
  constexpr LPCWSTR kWindowClassName = L"WispyEngineWindowClass";

  WNDCLASSEX window_class { 0 };
  window_class.cbSize = sizeof(WNDCLASSEX);
  window_class.style = CS_HREDRAW | CS_VREDRAW;
  window_class.lpfnWndProc = WndProc;
  window_class.cbClsExtra = 0;
  window_class.cbWndExtra = 0;
  window_class.hInstance = GetModuleHandle(NULL);
  window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
  window_class.hbrBackground = (HBRUSH) COLOR_WINDOW;
  window_class.lpszMenuName = NULL;
  window_class.lpszClassName = kWindowClassName;
  window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  if (!RegisterClassEx(&window_class)) {
    is_bad_ = true;
    MessageBoxEx(NULL, L"Window Class Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
    return;
  }

  // Create Window
  log_.Append("[INFO] Creating window...");

  hwnd_ = CreateWindowEx(0, kWindowClassName, name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

  if (hwnd_ == NULL) {
    MessageBoxEx(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
    is_bad_ = true;
    return;
  }

  // Fullscreen
  ActivateFullscreen();

  // Creating Graphics
  log_.Append("[INFO] Initializing Direct2D...");

  if (FAILED(graphics_.CreateGraphicsResources(hwnd_))) {
    is_bad_ = true;
    return;
  }

  // Fixing Resolution
  RECT client_rect = { 0 };
  GetClientRect(hwnd_, &client_rect);
  int client_w = client_rect.right - client_rect.left;
  int client_h = client_rect.bottom - client_rect.top;
  scale_ = static_cast<float>(Minimum(client_w / resolution_width, client_h / resolution_height));
  fixed_w_ = resolution_width * scale_, fixed_h_ = resolution_height * scale_;
  dis_x_ = (client_w - fixed_w_) / 2, dis_y_ = (client_h - fixed_h_) / 2;

  // Add to Finder
  finder_.insert({ hwnd_, this });
}

Application::~Application() {
  if (!is_bad_) {
    finder_.erase(hwnd_);

    bitmaps_.clear();
  }
}

void Application::Run(unsigned int target_fps) {
  // No MJs
  if (is_bad_) return;

  // Show Window
  ShowWindow(hwnd_, SW_NORMAL);

  // Create Game Loop
  log_.Append("[INFO] Starting game loop...");

  using clock = std::chrono::high_resolution_clock;
  using std::chrono::nanoseconds, std::chrono::milliseconds;

  constexpr int kOneBillion = 1000000000;

  nanoseconds timestep(kOneBillion / target_fps);

  auto start_time = clock::now();
  auto curr_time = clock::now();
  nanoseconds delta_time(0);

  nanoseconds lag(0);
  float frame_delta_time = 0.0f;

  milliseconds timer(0);
  unsigned int frame_count = 0u;
  unsigned int fps = 0u;

  MSG msg { 0 };
  msg.message = WM_NULL;
  while (msg.message != WM_QUIT) {
    curr_time = clock::now();
    delta_time = std::chrono::duration_cast<nanoseconds>(curr_time - start_time);
    timer += std::chrono::duration_cast<milliseconds>(curr_time - start_time);
    start_time = curr_time;

    lag += delta_time;
    frame_delta_time += delta_time.count();

    if (timer.count() > 1000) {
      fps = frame_count;
      frame_count = 0u;
      timer = milliseconds(0);
    }

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      while (lag >= timestep) {
        lag -= timestep;

        if (current_world_) {
          if (!current_world_->Update(frame_delta_time / kOneBillion)) DestroyWindow(hwnd_);
          adjust_to_update_ = true;
        }

        frame_count++;
        frame_delta_time = 0;
      }

      graphics_.BeginDraw();

      graphics_.Clear(D2D1::ColorF(0u));

      if (current_world_) HandleWorldGraphics();

      WCHAR text[50] = L"FPS: ";
      wcscat_s(text, std::to_wstring(fps).c_str());
      graphics_.DrawBasicText(text, 0.0f, 0.0f, 150.0f, 30.0f);

      graphics_.EndDraw();
    }
  }
}

LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_KEYDOWN:
  case WM_KEYUP:
    finder_.find(hwnd)->second->HandleInput(wParam, !(lParam >> 31));
    break;

  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  return 0;
}

void Application::ActivateFullscreen() const {
  DWORD style = GetWindowLong(hwnd_, GWL_STYLE);
  MONITORINFO monitor_info = { sizeof(monitor_info) };
  WINDOWPLACEMENT window_placement = { };
  if (GetWindowPlacement(hwnd_, &window_placement) && GetMonitorInfo(MonitorFromWindow(hwnd_, MONITOR_DEFAULTTOPRIMARY), &monitor_info)) {
    SetWindowLong(hwnd_, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
    SetWindowPos(hwnd_, HWND_TOP, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top, monitor_info.rcMonitor.right - monitor_info.rcMonitor.left, monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
  }
}

void Application::HandleInput(unsigned long long key, bool is_down) {
  if (!current_world_) return;
  switch (key) {
  case VK_LEFT:
    current_world_->GetInputManager().SetKeyDown(kLeft, is_down);
    break;
  case VK_RIGHT:
    current_world_->GetInputManager().SetKeyDown(kRight, is_down);
    break;
  case VK_UP:
    current_world_->GetInputManager().SetKeyDown(kUp, is_down);
    break;
  case VK_DOWN:
    current_world_->GetInputManager().SetKeyDown(kDown, is_down);
    break;
  case VK_ESCAPE:
    current_world_->GetInputManager().SetKeyDown(kEscape, is_down);
    break;
  }
}

void Application::HandleWorldGraphics() {
  // Retrieve and Load Sprites
  for (auto &sprite : current_world_->GetMainCamera().GetSpriteBuffer()) {
    if (const auto &bitmap = graphics_.LoadBitmapFromFilename(std::wstring(sprite.first.begin(), sprite.first.end()).c_str())) {
      bitmaps_.push_back(bitmap);
      if (!sprite.second.expired()) *sprite.second.lock() = static_cast<unsigned int>(bitmaps_.size());
      else {
        log_.Append("[ERROR] ID assignment to sprite for " + sprite.first + " failed!");
        bitmaps_.erase(bitmaps_.end());
      }
    } else {
      log_.Append("[ERROR] Sprite registration to " + sprite.first + " failed!");
    }
  }

  if (adjust_to_update_) {
    camera_sprite_data_ = current_world_->GetMainCamera().GetCameraSpriteDataStream();
  }

  // Drawing Resolution-Scaled Window
  graphics_.FillRectangle(dis_x_, dis_y_, fixed_w_, fixed_h_, D2D1::ColorF(1.0f, 1.0f, 1.0f));

  // Drawing Sprites
  for (int i = 0; i < camera_sprite_data_.size(); ++i) {
    if (camera_sprite_data_[i].sprite_id == 0u) continue;
    graphics_.DrawBitmap(bitmaps_[camera_sprite_data_[i].sprite_id - 1], camera_sprite_data_[i].x * scale_ + dis_x_, camera_sprite_data_[i].y * scale_ + dis_y_, static_cast<float>(scale_), static_cast<float>(scale_), camera_sprite_data_[i].slices_x, camera_sprite_data_[i].slices_y, camera_sprite_data_[i].index_x, camera_sprite_data_[i].index_y);
  }

  // Debug Draw
  if (adjust_to_update_) {
    debug_draw_data_ = current_world_->GetMainCamera().GetDebugDrawDataStream();
    adjust_to_update_ = false;
  }

  for (int i = 0; i < debug_draw_data_.size(); ++i) {
    switch (debug_draw_data_[i].draw_type) {
    case DebugDrawer::DrawData::DrawType::kRect:
      graphics_.DrawRectangle(debug_draw_data_[i].x * scale_ + dis_x_, debug_draw_data_[i].y * scale_ + dis_y_, debug_draw_data_[i].width * scale_, debug_draw_data_[i].height * scale_, D2D1::ColorF(debug_draw_data_[i].color.r, debug_draw_data_[i].color.g, debug_draw_data_[i].color.b));
      break;
    case DebugDrawer::DrawData::DrawType::kCircle:
      graphics_.DrawEllipse(debug_draw_data_[i].x * scale_ + dis_x_, debug_draw_data_[i].y * scale_ + dis_y_, debug_draw_data_[i].radius * scale_, debug_draw_data_[i].radius * scale_, D2D1::ColorF(debug_draw_data_[i].color.r, debug_draw_data_[i].color.g, debug_draw_data_[i].color.b));
      break;
    }
  }
}

}
