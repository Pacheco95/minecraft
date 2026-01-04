#include "Time.h"

namespace App {

DeltaTime Time::deltaTime() const {
  return m_deltaTime;
}

void Time::update() {
  const Uint64 now = SDL_GetPerformanceCounter();
  m_deltaTime = static_cast<DeltaTime>(now - m_lastFrameTime) / static_cast<DeltaTime>(SDL_GetPerformanceFrequency());
  m_lastFrameTime = now;
}
} // namespace App
