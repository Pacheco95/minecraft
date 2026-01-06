#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex {
  glm::vec3 position;
  glm::vec4 color; // RGBA
  glm::vec3 normal;
  glm::vec2 uv;
  glm::vec3 tangent;
  glm::vec3 bitangent;
  // Which other fields are relevant?
};

class Mesh {
public:
  Mesh() : VAO(0), VBO(0), EBO(0) {
  }

  void setup();

  void render() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
  }

private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int VAO, VBO, EBO; // OpenGL handles
};
