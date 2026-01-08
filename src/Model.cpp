#include "Model.h"

#include <spdlog/spdlog.h>

void Model::setup() {
  for (const auto &[mesh, material] : meshGroups) {
    if (mesh) {
      mesh->setup();
    }
  }
}

void Model::render(const RenderContext &ctx) {
  for (auto &[mesh, material] : meshGroups) {
    if (!mesh || !material) {
      SPDLOG_WARN("Empty mesh or material");
      continue;
    }

    const std::shared_ptr<App::Shader> shader = ctx.customShader.value_or(material->getShader());

    shader->use();

    // 1. Set Global/Scene Uniforms
    shader->set("uProjection", ctx.projectionMatrix);
    shader->set("uView", ctx.viewMatrix);
    shader->set("uModel", ctx.modelMatrix);
    shader->set("uViewPosition", ctx.cameraPosition);
    shader->set("uLight.position", ctx.lightPosition);
    shader->set("uLight.direction", ctx.lightDirection);
    shader->set("uLight.color", ctx.lightColor);

    shader->set("uLight.ambientColor", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    shader->set("uLight.diffuseColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    shader->set("uLight.specularColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // 2. Set Material-Specific Uniforms (Colors, Shininess, etc.)
    material->applyUniforms();

    // 3. Bind Textures
    material->bindTextures();

    // 4. Draw
    mesh->render(ctx.renderMode);
  }
}

void Model::addMeshGroup(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material) {
  meshGroups.push_back({mesh, material});
}
