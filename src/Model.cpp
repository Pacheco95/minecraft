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

    shader->set("uWorld.lightSourceCount", ctx.lights.size());

    for (unsigned int i = 0; i < ctx.lights.size(); ++i) {
      shader->set(std::format("uWorld.lights[{}].color", i), ctx.lights[i].color);
      shader->set(std::format("uWorld.lights[{}].intensity", i), ctx.lights[i].intensity);
      shader->set(std::format("uWorld.lights[{}].position", i), ctx.lights[i].position);
      shader->set(std::format("uWorld.lights[{}].direction", i), ctx.lights[i].direction);
      shader->set(std::format("uWorld.lights[{}].constant", i), ctx.lights[i].constant);
      shader->set(std::format("uWorld.lights[{}].linear", i), ctx.lights[i].linear);
      shader->set(std::format("uWorld.lights[{}].quadratic", i), ctx.lights[i].quadratic);
      shader->set(std::format("uWorld.lights[{}].innerCutoff", i), ctx.lights[i].innerCutoff);
      shader->set(std::format("uWorld.lights[{}].outerCutoff", i), ctx.lights[i].outerCutoff);

      // TODO: use actual light values
      // shader->set(std::format("uWorld.lights[{}].ambientColor", i), glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
      // shader->set(std::format("uWorld.lights[{}].diffuseColor", i), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
      // shader->set(std::format("uWorld.lights[{}].specularColor", i), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

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
