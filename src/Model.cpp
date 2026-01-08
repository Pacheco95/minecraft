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

    shader->set("uWorld.viewPosition", ctx.cameraPosition);

    shader->set("uWorld.light.color", ctx.light.color);
    shader->set("uWorld.light.intensity", ctx.light.intensity);
    shader->set("uWorld.light.position", ctx.light.position);
    shader->set("uWorld.light.direction", ctx.light.direction);
    shader->set("uWorld.light.constant", ctx.light.constant);
    shader->set("uWorld.light.linear", ctx.light.linear);
    shader->set("uWorld.light.quadratic", ctx.light.quadratic);
    shader->set("uWorld.light.innerCutoff", ctx.light.innerCutoff);
    shader->set("uWorld.light.outerCutoff", ctx.light.outerCutoff);

    shader->set("uWorld.light.ambientColor", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    shader->set("uWorld.light.diffuseColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    shader->set("uWorld.light.specularColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

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
