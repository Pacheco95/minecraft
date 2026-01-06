#pragma once

#include "glm/glm.hpp"

struct RenderContext {
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
};

class Renderable {
public:
  virtual ~Renderable() = default;

  virtual void setup() = 0;
  virtual void render(const RenderContext &ctx) = 0;
};
