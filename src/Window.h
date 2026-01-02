#pragma once

#include <SDL3/SDL.h>

namespace App {
struct Window {
  SDL_Window *m_sdlWindow = nullptr;
  SDL_Renderer *m_sdlRenderer = nullptr;
  SDL_GLContext m_glContext = nullptr;
};
} // namespace App
