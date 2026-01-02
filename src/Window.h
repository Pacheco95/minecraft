#pragma once

#include "Constants.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace App {
struct Window {
  SDL_Window *m_sdlWindow = nullptr;
  SDL_Renderer *m_sdlRenderer = nullptr;
  SDL_GLContext m_glContext = nullptr;

  SDL_AppResult setup();
  SDL_AppResult processEvent(const SDL_Event *event);
  void dispose();
  void render() const;

private:
  std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("bootstrap");
};

inline SDL_AppResult Window::setup() {
  SDL_SetAppMetadata("Minecraft", "0.1.0", "com.example.minecraft");

  if (!SDL_Init(SDL_INIT_VIDEO) != 0) {
    SPDLOG_CRITICAL("Couldn't initialize SDL: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Configure OpenGL attributes before creating the window/context
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  if (!SDL_CreateWindowAndRenderer("Minecraft", Config::Window::WIDTH,
                                   Config::Window::HEIGHT,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,
                                   &m_sdlWindow, &m_sdlRenderer)) {
    SPDLOG_CRITICAL("Couldn't create window/renderer: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetWindowPosition(m_sdlWindow, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);

  m_glContext = SDL_GL_CreateContext(m_sdlWindow);

  if (!m_glContext) {
    SPDLOG_CRITICAL("Couldn't create OpenGL context: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  const auto loader = reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress);

  if (!gladLoadGLLoader(loader)) {
    logger->critical("Couldn't initialize GLAD");
    return SDL_APP_FAILURE;
  }

  SDL_GL_MakeCurrent(m_sdlWindow, m_glContext);

  glViewport(0, 0, Config::Window::WIDTH, Config::Window::HEIGHT);

  SPDLOG_INFO("SDL and OpenGL initialized successfully");

  return SDL_APP_CONTINUE;
}
inline SDL_AppResult Window::processEvent(const SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  if (event->type == SDL_EVENT_WINDOW_RESIZED) {
    const auto width = event->window.data1;
    const auto height = event->window.data2;
    glViewport(0, 0, width, height);
    SPDLOG_DEBUG("Window resized: {} x {}", width, height);
    return SDL_APP_CONTINUE;
  }

  return SDL_APP_CONTINUE;
}

inline void Window::dispose() {
  if (m_glContext) {
    if (SDL_GL_DestroyContext(m_glContext)) {
      m_glContext = nullptr;
    } else {
      SPDLOG_ERROR("Failed to destroy GL context: {}", SDL_GetError());
    }
  }

  if (m_sdlRenderer) {
    SDL_DestroyRenderer(m_sdlRenderer);
    m_sdlRenderer = nullptr;
  }

  if (m_sdlWindow) {
    SDL_DestroyWindow(m_sdlWindow);
    m_sdlWindow = nullptr;
  }
}

inline void Window::render() const {
  constexpr float red = Config::Window::CLEAR_COLOR[0];
  constexpr float green = Config::Window::CLEAR_COLOR[1];
  constexpr float blue = Config::Window::CLEAR_COLOR[2];

  glClearColor(red, green, blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SDL_GL_SwapWindow(m_sdlWindow);
}
} // namespace App
