#include "sprite_renderer.h"

#include "world.h"

namespace wispy {

SpriteRenderer::SpriteRenderer(GameObject *parent) : Property(parent), renderer_id_(0), sprite_(), order_(0) { }

SpriteRenderer::~SpriteRenderer() {
  GetGameObject()->GetWorld()->GetMainCamera().RemoveRenderer(renderer_id_);
}

void SpriteRenderer::Start() {
  if (renderer_id_ == 0) {
    renderer_id_ = GetGameObject()->GetWorld()->GetMainCamera().AddRenderer(GetGameObject()->GetPropertyWeak<SpriteRenderer>());
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
