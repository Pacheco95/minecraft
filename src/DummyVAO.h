#pragma once

#include <glad/glad.h>

class DummyVAO {
public:
  DummyVAO();

  ~DummyVAO();

  void render() const;

  DummyVAO(const DummyVAO &) = delete;
  DummyVAO &operator=(const DummyVAO &) = delete;

  DummyVAO(DummyVAO &&) = delete;
  DummyVAO &operator=(DummyVAO &&) = delete;

private:
  GLuint m_VAO;
};
