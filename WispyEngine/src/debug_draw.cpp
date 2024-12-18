#include "debug_draw.h"

namespace wispy {

DebugDrawer::DebugDrawer() : draw_data_stream_() { }

void DebugDrawer::DrawRect(float x, float y, float width, float height, Color color) {
  DrawData draw_data = { 0 };

  draw_data.x = x, draw_data.y = y;
  draw_data.draw_type = DrawData::DrawType::kRect;
  draw_data.width = width, draw_data.height = height;
  draw_data.color = color;

  draw_data_stream_.push_back(draw_data);
}

void DebugDrawer::DrawCircle(float x, float y, float radius, Color color) {
  DrawData draw_data = { 0 };

  draw_data.x = x, draw_data.y = y;
  draw_data.draw_type = DrawData::DrawType::kCircle;
  draw_data.radius = radius;
  draw_data.color = color;

  draw_data_stream_.push_back(draw_data);
}

std::vector<DebugDrawer::DrawData> DebugDrawer::GetDrawDataStream() {
  std::vector<DrawData> stream;

  for (int i = 0; i < draw_data_stream_.size(); ++i) {
    stream.push_back(draw_data_stream_[i]);
  }

  draw_data_stream_.clear();

  return stream;
}

}
