#include "Model.h"

#include <spdlog/spdlog.h>

void Model::setup() {
}

void Model::render(const RenderContext &ctx) {
  for (auto &[mesh, material] : meshGroups) {
    if (!mesh || !material) {
      SPDLOG_WARN("Empty mesh or material");
      continue;
    }

    const std::shared_ptr<App::Shader> shader = material->getShader();
    shader->use();

    // 1. Set Global/Scene Uniforms
    shader->set("projection", ctx.projectionMatrix);
    shader->set("view", ctx.viewMatrix);
    shader->set("model", ctx.modelMatrix);

    // 2. Set Material-Specific Uniforms (Colors, Shininess, etc.)
    material->applyUniforms();

    // 3. Bind Textures
    material->bindTextures();

    // 4. Draw
    mesh->render();
  }
}
