#include "FloorGrid.h"

namespace App {

GLuint FloorGrid::gridVAO = 0;
GLuint FloorGrid::gridVBO = 0;
std::shared_ptr<Shader> FloorGrid::gridShader = {};

FloorGrid::~FloorGrid() {
  if (gridVAO) {
    glDeleteVertexArrays(1, &gridVAO);
  };

  if (gridVBO) {
    glDeleteBuffers(1, &gridVBO);
  }

  gridVAO = gridVBO = 0;
}

void FloorGrid::setup() {
  gridShader = ShaderCache::get("grid");

  if (!gridVAO) {
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  }
}

void FloorGrid::render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
  gridShader->use();
  gridShader->set("view", viewMatrix);
  gridShader->set("projection", projectionMatrix);

  // GRID RENDER STATES
  glDepthMask(GL_FALSE);
  glDisable(GL_CULL_FACE); // Disable culling to ensure full quad draws

  glBindVertexArray(gridVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  // Restore State for next frame/ImGui
  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
}
} // namespace App
