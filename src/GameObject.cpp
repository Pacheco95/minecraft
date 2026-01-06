#include "GameObject.h"

void GameObject::render(const RenderContext &ctx) const {
  if (renderer) {
    const RenderContext objectCtx = {transform.GetModelMatrix(), // From your Transform class
                                     ctx.viewMatrix, ctx.projectionMatrix};

    renderer->render(objectCtx);
  }
}
