#include <Windows.h>

#include "application.h"

#include "my_world.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow) {
  wispy::Application app(L"WayLand", 320, 180);

  app.SetCurrentWorld<MyWorld>();

  app.Run(60);

  return 0;
}
