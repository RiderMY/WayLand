#pragma once

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

  void SetUnitSize(int size);

private:
  CCS<SpriteRenderer> renderers_;
  std::vector<std::pair<std::string, std::weak_ptr<unsigned int>>> sprite_buffer_;

  int unit_size_;
};

struct CameraSpriteData {
  float x, y;
  int order;
  unsigned int sprite_id;
};

}
