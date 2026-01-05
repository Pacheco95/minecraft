#include "DummyVAO.h"

DummyVAO::DummyVAO() : m_VAO(0) {
  glGenVertexArrays(1, &m_VAO);
}

DummyVAO::~DummyVAO() {
  if (m_VAO) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }
}

void DummyVAO::render() const {
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
