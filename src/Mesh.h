#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

// 1 Single source of truth
#define VERTEX_FIELDS(X)                                                                                               \
  X(glm::vec3, position)                                                                                               \
  X(glm::vec4, color)                                                                                                  \
  X(glm::vec3, normal)                                                                                                 \
  X(glm::vec2, uv)                                                                                                     \
  X(glm::vec3, tangent)                                                                                                \
  X(glm::vec3, bitangent)

// 2 Struct generation
struct Vertex {
#define X(type, name) type name;
  VERTEX_FIELDS(X)
#undef X
};

// 3 Enum generation (same order)
enum VertexAttributeIndex {
#define X(type, name) name##AttrIndex,
  VERTEX_FIELDS(X)
#undef X
};

#undef VERTEX_FIELDS

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
      : vertices(std::move(vertices)), indices(std::move(indices)), VAO(0), VBO(0), EBO(0) {
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
