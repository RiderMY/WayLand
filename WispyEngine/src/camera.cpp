#include "camera.h"

#include "game_object.h"

namespace wispy {

Camera::Camera(GameObject *parent) : Property(parent), renderers_(), sprite_buffer_(), unit_size_(1) { }

int Camera::AddRenderer(std::weak_ptr<SpriteRenderer> renderer) {
  return renderers_.Add(renderer);
}

void Camera::RemoveRenderer(unsigned int renderer_id) {
  renderers_.Remove(renderer_id);
}

Sprite Camera::RegisterSprite(std::string filename) {
  Sprite sprite;

  sprite_buffer_.push_back({ filename, sprite.sprite_id });

  return sprite;
}

std::vector<std::pair<std::string, std::weak_ptr<unsigned int>>> Camera::GetSpriteBuffer() {
  std::vector<std::pair<std::string, std::weak_ptr<unsigned int>>> output_buffer;
  for (const auto &id : sprite_buffer_) {
    output_buffer.push_back(id);
  }
  sprite_buffer_.clear();
  return output_buffer;
}

std::vector<CameraSpriteData> Camera::GetCameraSpriteDataStream() {
  std::vector<CameraSpriteData> stream;
  
  std::vector<std::weak_ptr<SpriteRenderer>> renderer_stream = renderers_.GetStream();

  // Sort Data by Order (Least to Greatest)
  CameraSpriteData data = { 0 };
  for (int i = 0; i < renderer_stream.size(); ++i) {
    Vec2 position = renderer_stream[i].lock()->GetGameObject()->GetPosition() - GetGameObject()->GetPosition();
    data.x = position.x * unit_size_;
    data.y = position.y * unit_size_;
    data.order = renderer_stream[i].lock()->GetOrder();
    data.sprite_id = *renderer_stream[i].lock()->GetSprite().sprite_id;

    for (int j = 0; j < i + 1; ++j) {
      if (j == i) stream.push_back(data);
      else if (data.order < stream[j].order) {
        stream.insert(stream.begin() + j, data);
        break;
      }
    }
  }

  return stream;
}

void Camera::SetUnitSize(int size) {
  unit_size_ = size;
}

}
