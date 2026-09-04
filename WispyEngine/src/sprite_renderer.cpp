#include "sprite_renderer.h"

#include "world.h"

namespace wispy {

SpriteRenderer::SpriteRenderer(GameObject *parent, Sprite sprite, int order) : Property(parent), renderer_id_(0u), sprite_(sprite), order_(order) { }

SpriteRenderer::~SpriteRenderer() {
  GetGameObject()->GetWorld()->GetMainCamera().RemoveRenderer(renderer_id_);
}

void SpriteRenderer::Start() {
  if (renderer_id_ == 0u) {
    auto property = GetGameObject()->GetPropertyWeak<SpriteRenderer>();
    if (property.expired()) {
      GetGameObject()->GetWorld()->AppendToWorldLog("SpriteRenderer was prematurely removed from GameObject");
    } else {
      renderer_id_ = GetGameObject()->GetWorld()->GetMainCamera().AddRenderer(property);
    }
  }
}

void SpriteRenderer::SetOrder(int order) {
  order_ = order;
}

int SpriteRenderer::GetOrder() const {
  return order_;
}

void SpriteRenderer::SetSprite(Sprite sprite) {
  sprite_ = sprite;
}

Sprite SpriteRenderer::GetSprite() const {
  return sprite_;
}

}
