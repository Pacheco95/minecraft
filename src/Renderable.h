#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Light.h"

struct RenderContext {
  glm::mat4 modelMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  glm::vec3 cameraPosition;
  std::vector<Light> lights;
  GLuint renderMode = GL_TRIANGLES;
  std::optional<std::shared_ptr<App::Shader>> customShader = std::nullopt;
};

class Renderable {
public:
  virtual ~Renderable() = default;

  virtual void setup() = 0;
  virtual void render(const RenderContext &ctx) = 0;
};
