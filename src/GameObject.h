#pragma once

#include <memory>

#include "Renderable.h"
#include "Transform.h"

class GameObject {
public:
  void render(const RenderContext &ctx) const;

private:
  Transform transform;
  std::shared_ptr<Renderable> renderer = nullptr;
};
