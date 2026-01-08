#pragma once

#include <memory>

#include "Renderable.h"
#include "Transform.h"

class GameObject {
public:
  void render(const RenderContext &ctx) const;

private:
  Transform m_transform;
  std::shared_ptr<Renderable> m_renderer = nullptr;
};
