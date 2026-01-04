#pragma once

#include <SDL3/SDL.h>

namespace App {
using DeltaTime = float;

class Time {
public:
  [[nodiscard]] DeltaTime deltaTime() const;

  void update();

private:
  Uint64 m_lastFrameTime = 0;
  DeltaTime m_deltaTime = 0;
};

} // namespace App
