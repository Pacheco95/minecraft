#pragma once

#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "ShaderCache.h"

namespace App {
class FloorGrid {
public:
  ~FloorGrid();

  static void setup();

  static void render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

  static constexpr float quadVertices[] = {
      // Positions (2D is fine, Z=0)
      -1.0f, 1.0f,  0.0f, // Top-Left
      -1.0f, -1.0f, 0.0f, // Bot-Left
      1.0f,  1.0f,  0.0f, // Top-Right

      1.0f,  1.0f,  0.0f, // Top-Right
      -1.0f, -1.0f, 0.0f, // Bot-Left
      1.0f,  -1.0f, 0.0f  // Bot-Right
  };

private:
  static GLuint gridVAO, gridVBO;
  static std::shared_ptr<Shader> gridShader;
};

} // namespace App
