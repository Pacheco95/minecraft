#include "Axis.h"

Axis::~Axis() {
  if (m_VAO) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }

  if (m_VBO) {
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
  }
}

void Axis::setup() {
  if (m_VAO) {
    return;
  }

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
}

void Axis::render() const {
  glBindVertexArray(m_VAO);
  glLineWidth(3.0f);
  glDrawArrays(GL_LINES, 0, 6);
}
