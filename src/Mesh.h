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
      : m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_VAO(0), m_VBO(0), m_EBO(0) {
  }

  void setup();

  void render(GLuint renderMode = GL_TRIANGLES) const;

private:
  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;
  unsigned int m_VAO, m_VBO, m_EBO; // OpenGL handles
};
