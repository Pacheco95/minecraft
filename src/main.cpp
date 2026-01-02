#define SDL_MAIN_USE_CALLBACKS 1

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Constants.h"
#include "Engine.h"
#include "Window.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using namespace App;

auto logger = spdlog::stdout_color_mt("bootstrap");

constexpr Engine *asEngine(void *appstate) {
  auto *engine = static_cast<Engine *>(appstate);
  return engine;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  static Window window;
  static Engine engine(&window);

  *appstate = &engine;

  SDL_SetAppMetadata("Minecraft", "0.1.0", "com.example.minecraft");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    logger->error("Couldn't initialize SDL: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer(
          "Minecraft", Config::Window::WIDTH, Config::Window::HEIGHT,
          SDL_WINDOW_RESIZABLE, &window.m_sdlWindow, &window.m_sdlRenderer)) {
    logger->critical("Couldn't create window/renderer: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(window.m_sdlRenderer, Config::Window::WIDTH,
                                   Config::Window::HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  logger->info("SDL initialized successfully");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  const Engine *engine = asEngine(appstate);
  const auto &window = engine->getWindow();

  constexpr auto red = Config::Window::CLEAR_COLOR[0];
  constexpr auto green = Config::Window::CLEAR_COLOR[0];
  constexpr auto blue = Config::Window::CLEAR_COLOR[0];

  SDL_SetRenderDrawColorFloat(window->m_sdlRenderer, red, green, blue,
                              SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(window->m_sdlRenderer);
  SDL_RenderPresent(window->m_sdlRenderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, const SDL_AppResult result) {
  if (result == SDL_APP_SUCCESS) {
    logger->info("Application finished successfully");
  } else {
    logger->error("Application finished with error(s): {}", SDL_GetError());
  }
}
