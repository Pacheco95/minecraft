#define SDL_MAIN_USE_CALLBACKS 1

#include <spdlog/spdlog.h>

#include <glad/glad.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Engine.h"
#include "Window.h"

using namespace App;

Uint64 g_lastFrameTime = 0;

constexpr Engine *asEngine(void *appstate) {
  auto *engine = static_cast<Engine *>(appstate);
  return engine;
}

constexpr Window *asWindow(void *appstate) {
  return asEngine(appstate)->getWindow();
}

double getDeltaTime() {
  const auto now = SDL_GetPerformanceCounter();
  const double deltaTime =
      static_cast<double>(now - g_lastFrameTime) / static_cast<double>(SDL_GetPerformanceFrequency());
  g_lastFrameTime = now;
  return deltaTime;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::trace);

  static Window window;
  static Engine engine(&window);

  *appstate = &engine;

  const SDL_AppResult setupResult = window.setup();

  g_lastFrameTime = SDL_GetPerformanceCounter();

  return setupResult;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  return asWindow(appstate)->processEvent(event);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  const Engine *engine = asEngine(appstate);
  engine->processFrame(getDeltaTime());
  engine->getWindow()->render();
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, const SDL_AppResult result) {
  if (result == SDL_APP_SUCCESS) {
    SPDLOG_INFO("Application finished successfully");
  } else {
    SPDLOG_ERROR("Application finished with error(s): {}", SDL_GetError());
  }

  Window *windowPtr = asWindow(appstate);

  windowPtr->dispose();
}
