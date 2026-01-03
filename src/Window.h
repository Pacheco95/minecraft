#pragma once

#include <SDL3/SDL.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace App {



struct Window {
  SDL_AppResult setup();
  SDL_AppResult processEvent(const SDL_Event *event);
  void dispose();
  void render() const;

private:
  std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("bootstrap");
  SDL_Window *m_sdlWindow = nullptr;
  SDL_Renderer *m_sdlRenderer = nullptr;
  SDL_GLContext m_glContext = nullptr;

  void setupImgui() const;
  static void createImGuiWindows();
  static void renderScene();
};
} // namespace App
