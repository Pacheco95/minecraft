#pragma once

#include <glad/glad.h>

class Axis {

  struct Vertex {
    float position[3];
    float color[3];
  };

public:
  Axis() : m_VAO(0), m_VBO(0) {
  }

  ~Axis();

  void setup();

  void render() const;

private:
  GLuint m_VAO, m_VBO;
  static constexpr auto offset = 0.005f;
  static constexpr float vertices[] = {
      0.0f + offset, 0.0f, 0.0f + offset, 1.0f, 0.0f, 0.0f, // X
      1.0f + offset, 0.0f, 0.0f + offset, 1.0f, 0.0f, 0.0f, // X

      0.0f + offset, 0.0f, 0.0f + offset, 0.0f, 1.0f, 0.0f, // Y
      0.0f + offset, 1.0f, 0.0f + offset, 0.0f, 1.0f, 0.0f, // Y

      0.0f + offset, 0.0f, 0.0f + offset, 0.0f, 0.0f, 1.0f, // Z
      0.0f + offset, 0.0f, 1.0f + offset, 0.0f, 0.0f, 1.0f, // Z
  };
};
