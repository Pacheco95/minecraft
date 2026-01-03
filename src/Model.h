#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <spdlog/spdlog.h>

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
  std::string diffuseTexturePath;
  std::string normalTexturePath;
  std::string specularTexturePath;

  // OpenGL texture handles
  GLuint diffuseTextureID = 0;
  GLuint normalTextureID = 0;
  GLuint specularTextureID = 0;
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
  void renderMesh(size_t meshIndex) const;
  const std::vector<Mesh> &getMeshes() const;
  std::vector<Mesh> &getMeshes();
  size_t getMeshCount() const;
  const Mesh *getMesh(size_t index) const;
  Mesh *getMesh(size_t index);
  bool isLoaded() const;

private:
  std::vector<Mesh> meshes;
  std::string modelDirectory;

  void cleanup();

  void processNode(aiNode *node, const aiScene *scene);
  void processMesh(aiMesh *aiMesh, const aiScene *scene);
  Material processMaterial(aiMaterial *aiMaterial, const aiScene *scene) const;
  GLuint loadTexture(const std::string &texturePath) const;
};

} // namespace App
