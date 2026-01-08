#pragma once

#include <vector>
#include <memory>

#include "Material.h"
#include "Renderable.h"
#include "Mesh.h"

class Model : public Renderable {
public:
  void setup() override;
  void render(const RenderContext &ctx) override;
  void addMeshGroup(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material);

private:
  struct MeshGroup {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
  };

  std::vector<MeshGroup> m_meshGroups;
};
