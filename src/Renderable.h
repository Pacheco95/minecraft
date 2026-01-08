#pragma once

#include <glm/glm.hpp>

#include "Shader.h"

struct RenderContext {
  glm::mat4 modelMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  glm::vec3 cameraPosition;
  glm::vec3 lightPosition;
  glm::vec3 lightDirection;
  glm::vec4 lightColor;
  GLuint renderMode = GL_TRIANGLES;
  std::optional<std::shared_ptr<App::Shader>> customShader = std::nullopt;
};

class Renderable {
public:
  virtual ~Renderable() = default;

  virtual void setup() = 0;
  virtual void render(const RenderContext &ctx) = 0;
};
