#pragma once

#include <SDL3/SDL.h>

namespace App {
using DeltaTime = float;

class Time {
public:
  static DeltaTime deltaTime();

  static void update();

private:
  static Uint64 m_lastFrameTime;
  static DeltaTime m_deltaTime;
};

}
