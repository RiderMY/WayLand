#include "my_menu.h"

MyMenu::MyMenu() {
  GetMainCamera().SetUnitSize(16u);
}

bool MyMenu::Update(float dt) {
  if (GetInputManager().GetKeyDown(wispy::kEscape)) return false;

  if (GetInputManager().GetKeyDown(wispy::kF)) SetCurrentWorldIndex(1ull);

  return true;
}
