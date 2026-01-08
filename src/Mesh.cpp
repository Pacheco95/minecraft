#include "Mesh.h"

#define _offset(name) reinterpret_cast<void *>(offsetof(Vertex, name))
#define _size(name) decltype(Vertex::name)::length()

#define _bind(name, glType)                                                                                            \
  glEnableVertexAttribArray(name##AttrIndex);                                                                          \
  glVertexAttribPointer(name##AttrIndex, _size(name), glType, GL_FALSE, sizeof(Vertex), _offset(name))

void Mesh::setup() {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

  _bind(position, GL_FLOAT);
  _bind(color, GL_FLOAT);
  _bind(normal, GL_FLOAT);
  _bind(uv, GL_FLOAT);
  _bind(tangent, GL_FLOAT);
  _bind(bitangent, GL_FLOAT);

  glBindVertexArray(0);
}

void Mesh::render(const GLuint renderMode) const {
  glBindVertexArray(m_VAO);
  glDrawElements(renderMode, static_cast<GLuint>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

#undef _bind
#undef _size
#undef _offset
