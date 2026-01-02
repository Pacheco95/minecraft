#define SDL_MAIN_USE_CALLBACKS 1
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <glad/glad.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <GL/gl.h>

#include "Constants.h"
#include "Engine.h"
#include "Window.h"

using namespace App;

auto logger = spdlog::stdout_color_mt("bootstrap");

Uint64 g_lastFrameTime = 0;

constexpr Engine *asEngine(void *appstate) {
  auto *engine = static_cast<Engine *>(appstate);
  return engine;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::debug);
  static Window window;
  static Engine engine(&window);

  *appstate = &engine;

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

  if (!SDL_CreateWindowAndRenderer(
          "Minecraft", Config::Window::WIDTH, Config::Window::HEIGHT,
          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, &window.m_sdlWindow,
          &window.m_sdlRenderer)) {
    SPDLOG_CRITICAL("Couldn't create window/renderer: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  window.m_glContext = SDL_GL_CreateContext(window.m_sdlWindow);

  if (!window.m_glContext) {
    SPDLOG_CRITICAL("Couldn't create OpenGL context: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  const auto loader = reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress);

  if (!gladLoadGLLoader(loader)) {
    logger->critical("Couldn't initialize GLAD");
    return SDL_APP_FAILURE;
  }

  SDL_GL_MakeCurrent(window.m_sdlWindow, window.m_glContext);

  glViewport(0, 0, Config::Window::WIDTH, Config::Window::HEIGHT);

  g_lastFrameTime = SDL_GetPerformanceCounter();

  SPDLOG_INFO("SDL and OpenGL initialized successfully");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
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

SDL_AppResult SDL_AppIterate(void *appstate) {
  const auto now = SDL_GetPerformanceCounter();
  const double deltaTime = static_cast<double>(now - g_lastFrameTime) /
                           static_cast<double>(SDL_GetPerformanceFrequency());
  g_lastFrameTime = now;

  const Engine *engine = asEngine(appstate);
  engine->processFrame(deltaTime);
  const auto windowPtr = engine->getWindow();

  constexpr float red = Config::Window::CLEAR_COLOR[0];
  constexpr float green = Config::Window::CLEAR_COLOR[1];
  constexpr float blue = Config::Window::CLEAR_COLOR[2];

  glClearColor(red, green, blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SDL_GL_SwapWindow(windowPtr->m_sdlWindow);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, const SDL_AppResult result) {
  if (result == SDL_APP_SUCCESS) {
    SPDLOG_INFO("Application finished successfully");
  } else {
    SPDLOG_ERROR("Application finished with error(s): {}", SDL_GetError());
  }

  const Engine *engine = asEngine(appstate);
  Window *windowPtr = engine->getWindow();

  if (windowPtr->m_glContext) {
    if (SDL_GL_DestroyContext(windowPtr->m_glContext)) {
      windowPtr->m_glContext = nullptr;
    } else {
      SPDLOG_ERROR("Failed to destroy GL context: {}", SDL_GetError());
    }
  }

  if (windowPtr->m_sdlRenderer) {
    SDL_DestroyRenderer(windowPtr->m_sdlRenderer);
    windowPtr->m_sdlRenderer = nullptr;
  }

  if (windowPtr->m_sdlWindow) {
    SDL_DestroyWindow(windowPtr->m_sdlWindow);
    windowPtr->m_sdlWindow = nullptr;
  }
}
