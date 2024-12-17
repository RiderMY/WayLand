#pragma once

#include <functional>

#include "ccs.h"
#include "property.h"
#include "sprite_renderer.h"

namespace wispy {

struct CameraSpriteData;

class Camera : public Property {
public:
  Camera(GameObject *parent);

  int AddRenderer(std::weak_ptr<SpriteRenderer> renderer);
  void RemoveRenderer(unsigned int renderer_id);

  Sprite RegisterSprite(std::string filename);
  std::vector<std::pair<std::string, std::weak_ptr<unsigned int>>> GetSpriteBuffer();
  std::vector<CameraSpriteData> GetCameraSpriteDataStream();

  unsigned int AddDebugDraw(std::function<void(DebugDrawer &)> debug_draw);
  void RemoveDebugDraw(unsigned int debug_draw_id);
  std::vector<DebugDrawer::DrawData> GetDebugDrawDataStream();

  void SetUnitSize(int size);

private:
  CCS<std::weak_ptr<SpriteRenderer>> renderers_;
  std::vector<std::pair<std::string, std::weak_ptr<unsigned int>>> sprite_buffer_;

  CCS<std::function<void(DebugDrawer &)>> debug_draws_;

  int unit_size_;
};

struct CameraSpriteData {
  float x, y;
  int order;
  unsigned int sprite_id;
};

}
