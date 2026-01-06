#include "Model.h"

#include <spdlog/spdlog.h>

void Model::setup() {
}

void Model::render(const RenderContext &ctx) {
  for (auto &[mesh, material] : meshGroups) {
    if (!mesh || !material) {
      SPDLOG_WARN("Empty mesh or material");
    };

    // 1. Prepare the Shader (stored in the material)
    const std::shared_ptr<App::Shader> shader = material->getShader();
    shader->use();

    // 2. Set Transformation Uniforms
    shader->set("projection", ctx.projectionMatrix);
    shader->set("view", ctx.viewMatrix);
    shader->set("model", ctx.modelMatrix);

    // 3. Bind Textures from the Material
    material->bindTextures();

    // 4. Draw the Mesh
    mesh->render();
  }
}
