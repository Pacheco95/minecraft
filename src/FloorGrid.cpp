#include "FloorGrid.h"

#include "Container.h"

namespace App {

GLuint FloorGrid::s_gridVAO = 0;
GLuint FloorGrid::s_gridVBO = 0;
std::shared_ptr<Shader> FloorGrid::s_gridShader = {};

FloorGrid::~FloorGrid() {
  if (s_gridVAO) {
    glDeleteVertexArrays(1, &s_gridVAO);
  };

  if (s_gridVBO) {
    glDeleteBuffers(1, &s_gridVBO);
  }

  s_gridVAO = s_gridVBO = 0;
}

void FloorGrid::setup() {
  s_gridShader = g_shaderCache.get("grid");

  if (!s_gridVAO) {
    glGenVertexArrays(1, &s_gridVAO);
    glGenBuffers(1, &s_gridVBO);
    glBindVertexArray(s_gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  }
}

void FloorGrid::render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
  s_gridShader->use();
  s_gridShader->set("view", viewMatrix);
  s_gridShader->set("projection", projectionMatrix);

  // GRID RENDER STATES
  glDepthMask(GL_FALSE);
  glDisable(GL_CULL_FACE); // Disable culling to ensure full quad draws

  glBindVertexArray(s_gridVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  // Restore State for next frame/ImGui
  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
}
} // namespace App
