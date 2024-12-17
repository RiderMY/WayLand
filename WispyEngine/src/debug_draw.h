#pragma once

#include <vector>

namespace wispy {

struct Color {
  float r, g, b;
};

class DebugDrawer {
public:
  struct DrawData {
    float x, y, width, height;
    Color color;
  };

  DebugDrawer();

  void DrawRect(float x, float y, float width, float height, Color color);

  std::vector<DrawData> GetDrawDataStream();

private:
  std::vector<DrawData> draw_data_stream_;
};

}
