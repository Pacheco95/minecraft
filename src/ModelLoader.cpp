#include "ModelLoader.h"

#include <spdlog/spdlog.h>

std::shared_ptr<Model> ModelLoader::Load(const std::string &path) {
  Assimp::Importer importer;

  // Load with common optimizations: Triangulate, Flip UVs, and calculate Tangents
  const aiScene *scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
                                  aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    SPDLOG_ERROR("Assimp Error: {}", importer.GetErrorString());
    return nullptr;
  }

  const std::string directory = path.substr(0, path.find_last_of('/'));
  auto model = std::make_shared<Model>();

  processNode(scene->mRootNode, scene, model, directory);

  // Finalize the model by setting up GPU buffers for all meshes
  model->setup(); // Calls Mesh::setup() for all internal meshes
  return model;
}

void ModelLoader::processNode(const aiNode *node, const aiScene *scene, std::shared_ptr<Model> model,
                              const std::string &directory) {
  // 1. Process all the meshes attached to this specific node
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    // node->mMeshes contains indices to the actual meshes in the scene object
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

    // Convert Assimp mesh to your Mesh class
    auto myMesh = processMesh(mesh, scene);

    // Get the material for this mesh
    auto myMaterial = std::make_shared<Material>();

    // If the AI_SCENE_FLAGS_INCOMPLETE flag is not set there will always be at least ONE material.
    // So there's no need to check if mesh->mMaterialIndex is valid because it will always exist
    const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    myMaterial = loadMaterial(material, directory);

    // Add the pair to your Model's meshGroups
    // Note: You may need a public method like model->addMeshGroup(mesh, material)
    model->addMeshGroup(myMesh, myMaterial);
  }

  // 2. Recursively process each of this node's children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, model, directory);
  }
}

std::shared_ptr<Mesh> ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  vertices.reserve(mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex{};

    // Map Assimp vectors to your glm::vec4/vec3 fields in Vertex struct
    vertex.position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);

    if (mesh->mNormals)
      vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

    if (mesh->mTextureCoords[0]) {
      vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
      vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    if (face.mNumIndices != 3) {
      SPDLOG_WARN("Face has {} indices instead of 3 (should be triangulated)", face.mNumIndices);
    }

    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  // You'll need to update your Mesh.h to accept data in a constructor or setter
  return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Material> ModelLoader::loadMaterial(const aiMaterial *mat, const std::string &directory) {
  auto material = std::make_shared<Material>();

#define loadColorUniform(colorKey, uniformName)                                                                        \
  if (aiColor4D color; mat->Get(colorKey, color) == AI_SUCCESS) {                                                      \
    material->setUniform(uniformName, glm::vec4(color.r, color.g, color.b, color.a));                                  \
  }

  loadColorUniform(AI_MATKEY_COLOR_DIFFUSE, DIFFUSE_COLOR_UNIFORM_NAME);
  loadColorUniform(AI_MATKEY_COLOR_AMBIENT, AMBIENT_COLOR_UNIFORM_NAME);
  loadColorUniform(AI_MATKEY_COLOR_SPECULAR, SPECULAR_COLOR_UNIFORM_NAME);
  loadColorUniform(AI_MATKEY_COLOR_EMISSIVE, EMISSIVE_COLOR_UNIFORM_NAME);
  loadColorUniform(AI_MATKEY_COLOR_TRANSPARENT, TRANSPARENT_COLOR_UNIFORM_NAME);
  loadColorUniform(AI_MATKEY_COLOR_REFLECTIVE, REFLECTIVE_COLOR_UNIFORM_NAME);
#undef loadColorUniform

  // Helper to load a specific texture type
  auto loadTex = [&](const aiTextureType type) -> std::shared_ptr<Texture2D> {
    if (mat->GetTextureCount(type) > 0) {
      aiString texturePath;
      mat->GetTexture(type, 0, &texturePath);
      std::string fullPath = directory + "/" + texturePath.C_Str();

      auto tex = std::make_shared<Texture2D>(fullPath);
      tex->load(); // Preload from disk to GPU immediately
      return tex;
    }
    return nullptr;
  };

  // Map Assimp types to your Material slots
  material->setDiffuseTex(loadTex(aiTextureType_DIFFUSE));
  material->setSpecularTex(loadTex(aiTextureType_SPECULAR));
  material->setNormalTex(loadTex(aiTextureType_HEIGHT)); // Assimp often uses HEIGHT for normal maps in OBJs

  return material;
}
