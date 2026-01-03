#include "Time.h"

namespace App {
Uint64 Time::m_lastFrameTime = 0;
DeltaTime Time::m_deltaTime = 0;

DeltaTime Time::deltaTime() {
  return m_deltaTime;
}

void Time::update() {
  const auto now = SDL_GetPerformanceCounter();

  m_deltaTime = static_cast<DeltaTime>(now - m_lastFrameTime) / static_cast<DeltaTime>(SDL_GetPerformanceFrequency());
  m_lastFrameTime = now;
}
} // namespace App
