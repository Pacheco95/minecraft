#include "Mesh.h"

#define _offset(name) reinterpret_cast<void *>(offsetof(Vertex, name))
#define _size(name) decltype(Vertex::name)::length()

#define _bind(name, glType)                                                                                            \
  glEnableVertexAttribArray(name##AttrIndex);                                                                          \
  glVertexAttribPointer(name##AttrIndex, _size(name), glType, GL_FALSE, sizeof(Vertex), _offset(name))

void Mesh::setup() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  _bind(position, GL_FLOAT);
  _bind(color, GL_FLOAT);
  _bind(normal, GL_FLOAT);
  _bind(uv, GL_FLOAT);
  _bind(tangent, GL_FLOAT);
  _bind(bitangent, GL_FLOAT);

  glBindVertexArray(0);
}

#undef _size
#undef _offset
#undef _bind
