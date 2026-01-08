#include "GameObject.h"

void GameObject::render(const RenderContext &ctx) const {
  if (!m_renderer) {
    SPDLOG_WARN("No renderer");
  }

  const RenderContext objectCtx = {
      .modelMatrix = m_transform.GetModelMatrix(),
      .viewMatrix = ctx.viewMatrix,
      .projectionMatrix = ctx.projectionMatrix,
      .cameraPosition = ctx.cameraPosition,
  };

  m_renderer->render(objectCtx);
}
