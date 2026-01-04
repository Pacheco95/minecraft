#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <spdlog/spdlog.h>

#include "Texture2D.h"

namespace App {

// Vertex structure for flexible data storage
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;
  glm::vec2 texCoords;
};

// Material structure
struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float shininess;

  std::shared_ptr<Texture2D> diffuseTexture;
  std::shared_ptr<Texture2D> normalTexture;
  std::shared_ptr<Texture2D> specularTexture;
};

// Mesh structure with OpenGL handles
struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material material;

  // OpenGL handles
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;

  // Constructor and cleanup
  Mesh() = default;

  ~Mesh() {
    cleanup();
  }

  void setupBuffers();
  void render() const;
  void cleanup();
};

class Model {
public:
  ~Model();
  void load(const std::string &filePath);
  void setupAllBuffers();
  void render() const;
  [[nodiscard]] bool isLoaded() const;

private:
  std::vector<Mesh> meshes;
  std::string modelDirectory;

  void cleanup();

  void processNode(const aiNode *node, const aiScene *scene);
  void processMesh(aiMesh *aiMesh, const aiScene *scene);
  Material processMaterial(const aiMaterial *aiMaterial) const;
};

} // namespace App
