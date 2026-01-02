#include <vector>
#include <cmath>
#include "Sphere.h"

#include <glad/glad.h>

namespace App {

Sphere::Sphere(const float radius, const unsigned int latSegments, const unsigned int longSegments) {
  generate(radius, latSegments, longSegments);
  setupOpenGl();
}

Sphere::~Sphere() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

inline void Sphere::generate(const float radius, unsigned int latSegments, unsigned int longSegments) {
  vertices.clear();
  normals.clear();
  indices.clear();

  if (latSegments < 2)
    latSegments = 2;

  if (longSegments < 3)
    longSegments = 3;

  const unsigned int latCount = latSegments;
  const unsigned int lonCount = longSegments;

  // Generate vertices and normals
  for (unsigned int i = 0; i <= latCount; ++i) {
    const float theta = static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(latCount);
    const float sinTheta = std::sin(theta);
    const float cosTheta = std::cos(theta);

    for (unsigned int j = 0; j <= lonCount; ++j) {
      const float phi = 2.0f * static_cast<float>(M_PI) * static_cast<float>(j) / static_cast<float>(lonCount);
      const float sinPhi = std::sin(phi);
      const float cosPhi = std::cos(phi);

      const float x = radius * sinTheta * cosPhi;
      const float y = radius * cosTheta;
      const float z = radius * sinTheta * sinPhi;

      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);

      // normal for a sphere centered at origin is the normalized position
      const float nx = sinTheta * cosPhi;
      const float ny = cosTheta;
      const float nz = sinTheta * sinPhi;

      normals.push_back(nx);
      normals.push_back(ny);
      normals.push_back(nz);
    }
  }

  // Generate indices (two triangles per quad)
  for (unsigned int i = 0; i < latCount; ++i) {
    for (unsigned int j = 0; j < lonCount; ++j) {
      const unsigned int a = i * (lonCount + 1) + j;
      const unsigned int b = (i + 1) * (lonCount + 1) + j;

      // triangle 1
      indices.push_back(a);
      indices.push_back(b);
      indices.push_back(a + 1);

      // triangle 2
      indices.push_back(a + 1);
      indices.push_back(b);
      indices.push_back(b + 1);
    }
  }
}

void Sphere::render(const std::shared_ptr<Shader> shader) const {
  shader->use();
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Sphere::setupOpenGl() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex
  // buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
  // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when
  // it's not directly necessary.
  glBindVertexArray(0);
}

} // namespace App
