#include <Windows.h>

#include "application.h"

#include "my_world.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow) {
  wispy::Application app(L"WayLand", 320u, 180u);

  app.SetCurrentWorld<MyWorld>();

  app.Run(60u);

  return 0;
}
