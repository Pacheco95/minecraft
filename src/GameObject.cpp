#include "GameObject.h"

void GameObject::render(const RenderContext &ctx) const {
  if (!renderer) {
    SPDLOG_WARN("No renderer");
  }

  const RenderContext objectCtx = {
      .modelMatrix = transform.GetModelMatrix(),
      .viewMatrix = ctx.viewMatrix,
      .projectionMatrix = ctx.projectionMatrix,
      .cameraPosition = ctx.cameraPosition,
  };

  renderer->render(objectCtx);
}
