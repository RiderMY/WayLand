#pragma once

#include "property.h"
#include "sprite.h"

namespace wispy {

class SpriteRenderer : public Property {
public:
  SpriteRenderer(GameObject *parent);
  ~SpriteRenderer();

  void Start();

  void SetSprite(Sprite sprite);
  Sprite GetSprite() const;

  void SetOrder(int order);
  int GetOrder() const;

private:
  unsigned int renderer_id_;

  Sprite sprite_;
  int order_;
};

}
