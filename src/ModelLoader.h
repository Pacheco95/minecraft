#pragma once

#include <string>
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "Mesh.h"
#include "Material.h"

class ModelLoader {
public:
  static std::shared_ptr<Model> Load(const std::string &path);

private:
  // Helper to process Assimp nodes recursively
  static void processNode(const aiNode *node, const aiScene *scene, std::shared_ptr<Model> model,
                          const std::string &directory);

  // Helper to convert Assimp mesh to your Mesh class
  static std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);

  // Helper to load materials and textures
  static std::shared_ptr<Material> loadMaterial(const aiMaterial *mat, const std::string &directory);
};
