#pragma once

#include <SDL3/SDL.h>

namespace App {

struct Window {
  SDL_AppResult setup();
  static SDL_AppResult processEvent(const SDL_Event *event);
  void dispose();
  void render() const;

private:
  SDL_Window *m_sdlWindow = nullptr;
  SDL_Renderer *m_sdlRenderer = nullptr;
  SDL_GLContext m_glContext = nullptr;

  static void renderOpenGlData();
};
} // namespace App
