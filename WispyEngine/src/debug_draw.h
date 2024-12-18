#pragma once

#include <vector>

namespace wispy {

struct Color {
  float r, g, b;
};

class DebugDrawer {
public:
  struct DrawData {
    float x, y;
    enum DrawType {
      kRect, kCircle
    } draw_type;
    union {
      struct {
        float width, height;
      };
      float radius;
    };
    Color color;
  };

  DebugDrawer();

  void DrawRect(float x, float y, float width, float height, Color color);
  void DrawCircle(float x, float y, float radius, Color color);

  std::vector<DrawData> GetDrawDataStream();

private:
  std::vector<DrawData> draw_data_stream_;
};

}
