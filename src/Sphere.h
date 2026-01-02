#pragma once

#include "Shader.h"

#include <vector>

namespace App {

class Sphere {
public:
  explicit Sphere(float radius = 1.0f, unsigned int latSegments = 16, unsigned int longSegments = 32);

  ~Sphere();

  void generate(float radius, unsigned int latSegments, unsigned int longSegments);

  const std::vector<float> &getVertices() const {
    return vertices;
  }

  const std::vector<float> &getNormals() const {
    return normals;
  }

  const std::vector<unsigned int> &getIndices() const {
    return indices;
  }

  void render(std::shared_ptr<Shader> shader) const;

private:
  std::vector<float> vertices;       // x,y,z triples
  std::vector<float> normals;        // x,y,z triples
  std::vector<unsigned int> indices; // triangle indices
  unsigned int VBO, VAO, EBO;

  void setupOpenGl();
};
} // namespace App
