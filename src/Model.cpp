#include "Model.h"

#include <stb_image.h>

#include <filesystem>

namespace App {

void Mesh::setupBuffers() {
  if (vertices.empty() || indices.empty()) {
    SPDLOG_WARN("Cannot setup buffers for empty mesh");
    return;
  }

  // Generate and bind VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Create and bind VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  // Create and bind EBO
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  // Setup vertex attributes
  // Position (location 0)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

  // Normal (location 1)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

  // Color (location 2)
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

  // Texture Coordinates (location 3)
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

  // Unbind VAO
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  SPDLOG_DEBUG("Mesh buffers setup: VAO={}, VBO={}, EBO={}", VAO, VBO, EBO);
}

void Mesh::render() const {
  if (VAO == 0) {
    SPDLOG_WARN("Mesh not initialized, call setupBuffers() first");
    return;
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void Mesh::cleanup() {
  if (VAO != 0)
    glDeleteVertexArrays(1, &VAO);
  if (VBO != 0)
    glDeleteBuffers(1, &VBO);
  if (EBO != 0)
    glDeleteBuffers(1, &EBO);
  VAO = VBO = EBO = 0;
}

Model::~Model() {
  cleanup();
}

void Model::load(const std::string &filePath) {
  cleanup();

  // Store the directory path for relative texture paths
  modelDirectory = std::filesystem::path(filePath).parent_path().string();

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      filePath,
      aiProcess_Triangulate |        // Ensure all meshes are triangulated
      aiProcess_GenNormals |         // Generate normals if not present
      aiProcess_FlipUVs |            // Flip UVs for OpenGL
      aiProcess_CalcTangentSpace |   // Calculate tangent space for normal mapping
      aiProcess_JoinIdenticalVertices // Join identical vertices
  );

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    SPDLOG_ERROR("Failed to load model: {} - {}", filePath, importer.GetErrorString());
    return;
  }

  if (scene->mNumMeshes == 0) {
    SPDLOG_WARN("Model file {} contains no meshes", filePath);
    return;
  }

  SPDLOG_INFO("Loading model: {} with {} meshes", filePath, scene->mNumMeshes);
  processNode(scene->mRootNode, scene);
  SPDLOG_INFO("Successfully loaded model with {} meshes", meshes.size());
}

void Model::setupAllBuffers() {
  for (auto &mesh : meshes) {
    mesh.setupBuffers();
  }
  SPDLOG_INFO("Setup buffers for {} meshes", meshes.size());
}

void Model::render() const {
  for (const auto &mesh : meshes) {
    mesh.render();
  }
}

void Model::renderMesh(size_t meshIndex) const {
  if (meshIndex >= meshes.size()) {
    SPDLOG_WARN("Mesh index {} out of range", meshIndex);
    return;
  }
  meshes[meshIndex].render();
}

const std::vector<Mesh> &Model::getMeshes() const {
  return meshes;
}

std::vector<Mesh> &Model::getMeshes() {
  return meshes;
}

size_t Model::getMeshCount() const {
  return meshes.size();
}

const Mesh *Model::getMesh(size_t index) const {
  if (index >= meshes.size())
    return nullptr;
  return &meshes[index];
}

Mesh *Model::getMesh(size_t index) {
  if (index >= meshes.size())
    return nullptr;
  return &meshes[index];
}

bool Model::isLoaded() const {
  return !meshes.empty();
}

void Model::cleanup() {
  // Clean up textures
  for (auto &mesh : meshes) {
    if (mesh.material.diffuseTextureID != 0)
      glDeleteTextures(1, &mesh.material.diffuseTextureID);
    if (mesh.material.normalTextureID != 0)
      glDeleteTextures(1, &mesh.material.normalTextureID);
    if (mesh.material.specularTextureID != 0)
      glDeleteTextures(1, &mesh.material.specularTextureID);
  }
  meshes.clear();
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  // Process all meshes at this node
  for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
    aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(aiMesh, scene);
  }

  // Recursively process child nodes
  for (unsigned int i = 0; i < node->mNumChildren; ++i) {
    processNode(node->mChildren[i], scene);
  }
}

void Model::processMesh(aiMesh *aiMesh, const aiScene *scene) {
  Mesh mesh;

  // Process vertices
  mesh.vertices.reserve(aiMesh->mNumVertices);

  for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) {
    Vertex vertex;

    // Position
    vertex.position = glm::vec3(
        aiMesh->mVertices[i].x,
        aiMesh->mVertices[i].y,
        aiMesh->mVertices[i].z
    );

    // Normal (should be generated by aiProcess_GenNormals if not present)
    if (aiMesh->HasNormals()) {
      vertex.normal = glm::vec3(
          aiMesh->mNormals[i].x,
          aiMesh->mNormals[i].y,
          aiMesh->mNormals[i].z
      );
    } else {
      vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // Color (if available, otherwise white)
    if (aiMesh->HasVertexColors(0)) {
      vertex.color = glm::vec3(
          aiMesh->mColors[0][i].r,
          aiMesh->mColors[0][i].g,
          aiMesh->mColors[0][i].b
      );
    } else {
      vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    // Texture Coordinates
    if (aiMesh->HasTextureCoords(0)) {
      vertex.texCoords = glm::vec2(
          aiMesh->mTextureCoords[0][i].x,
          aiMesh->mTextureCoords[0][i].y
      );
    } else {
      vertex.texCoords = glm::vec2(0.0f, 0.0f);
    }

    mesh.vertices.emplace_back(vertex);
  }

  // Process indices (faces)
  mesh.indices.reserve(aiMesh->mNumFaces * 3); // Assuming triangles

  for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i) {
    aiFace face = aiMesh->mFaces[i];

    if (face.mNumIndices != 3) {
      SPDLOG_WARN("Face has {} indices instead of 3 (should be triangulated)", face.mNumIndices);
    }

    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
      mesh.indices.push_back(face.mIndices[j]);
    }
  }

  // Process material
  if (aiMesh->mMaterialIndex >= 0) {
    aiMaterial *aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
    mesh.material = processMaterial(aiMat, scene);
  } else {
    // Default material
    mesh.material.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    mesh.material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    mesh.material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    mesh.material.shininess = 32.0f;
  }

  SPDLOG_DEBUG("Processed mesh: {} vertices, {} indices, material: {}",
               mesh.vertices.size(), mesh.indices.size(),
               mesh.material.diffuseTexturePath.empty() ? "default" : mesh.material.diffuseTexturePath);

  meshes.emplace_back(mesh);
}

Material Model::processMaterial(aiMaterial *aiMat, const aiScene *scene) const {
  Material material;

  // Get diffuse color
  aiColor3D diffuse(0.0f, 0.0f, 0.0f);
  aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
  material.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);

  // Get ambient color
  aiColor3D ambient(0.0f, 0.0f, 0.0f);
  aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
  material.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);

  // Get specular color
  aiColor3D specular(0.5f, 0.5f, 0.5f);
  aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
  material.specular = glm::vec3(specular.r, specular.g, specular.b);

  // Get shininess
  float shininess = 32.0f;
  aiMat->Get(AI_MATKEY_SHININESS, shininess);
  material.shininess = shininess;

  // Get texture paths
  aiString texPath;

  // Diffuse texture
  if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
    aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
    material.diffuseTexturePath = std::string(texPath.C_Str());
    material.diffuseTextureID = loadTexture(material.diffuseTexturePath);
  }

  // Normal texture
  if (aiMat->GetTextureCount(aiTextureType_HEIGHT) > 0) {
    aiMat->GetTexture(aiTextureType_HEIGHT, 0, &texPath);
    material.normalTexturePath = std::string(texPath.C_Str());
    material.normalTextureID = loadTexture(material.normalTexturePath);
  }

  // Specular texture
  if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
    aiMat->GetTexture(aiTextureType_SPECULAR, 0, &texPath);
    material.specularTexturePath = std::string(texPath.C_Str());
    material.specularTextureID = loadTexture(material.specularTexturePath);
  }

  SPDLOG_DEBUG("Processed material: diffuse=({}, {}, {}), ambient=({}, {}, {}), "
               "specular=({}, {}, {}), shininess={}",
               material.diffuse.x, material.diffuse.y, material.diffuse.z,
               material.ambient.x, material.ambient.y, material.ambient.z,
               material.specular.x, material.specular.y, material.specular.z,
               material.shininess);

  return material;
}

GLuint Model::loadTexture(const std::string &texturePath) const {
  if (texturePath.empty()) {
    SPDLOG_WARN("Texture path is empty");
    return 0;
  }

  // Construct full path
  std::string fullPath = modelDirectory + "/" + texturePath;

  // Check if file exists
  if (!std::filesystem::exists(fullPath)) {
    SPDLOG_WARN("Texture file not found: {}", fullPath);
    return 0;
  }

  int width, height, channels;
  unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);

  if (!data) {
    SPDLOG_ERROR("Failed to load texture: {}", fullPath);
    return 0;
  }

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Set texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Determine format based on channel count
  GLenum format = GL_RGB;
  if (channels == 1) format = GL_RED;
  else if (channels == 3) format = GL_RGB;
  else if (channels == 4) format = GL_RGBA;

  // Upload texture data
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Free image data
  stbi_image_free(data);

  SPDLOG_DEBUG("Loaded texture: {} ({}x{}, {} channels) - ID: {}",
               fullPath, width, height, channels, textureID);

  return textureID;
}

} // namespace App
