#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <assimp/postprocess.h>
#include <spdlog/spdlog.h>

#include "Texture.h"

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

  std::shared_ptr<Texture> diffuseTexture;
  std::shared_ptr<Texture> normalTexture;
  std::shared_ptr<Texture> specularTexture;
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

class OldModel {
public:
  ~OldModel();
  void load(const std::string &filePath);
  void setupAllBuffers();
  void render() const;
  [[nodiscard]] bool isLoaded() const;

private:
  std::vector<Mesh> m_meshes;
  std::string m_modelDirectory;

  void cleanup();

  void processNode(const aiNode *node, const aiScene *scene);
  void processMesh(aiMesh *aiMesh, const aiScene *scene);
  Material processMaterial(const aiMaterial *aiMaterial) const;
};

} // namespace App
